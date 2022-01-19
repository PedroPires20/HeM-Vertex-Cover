#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <cassert>
#include <numeric>
#include <random>
#include <chrono>
#include "../include/graph.h"
#include "../include/utility.h"

using namespace std;

const double epsilon = 1e-9;

mt19937 rng((int) chrono::steady_clock::now().time_since_epoch().count());

int uniform(int l, int r){
	uniform_int_distribution<int> uid(l, r);
	return uid(rng);
}

// O(V.log(V) + E)
int lr(const InstanceInfo& instance) {
    vector<bool> seen(instance.num_vertexes, false), in_vc(instance.num_vertexes, false);
    vector<int> L(instance.num_vertexes);
    iota(L.begin(), L.end(), 0);
    stable_sort(L.rbegin(), L.rend(), [&] (int u, int v) {
        return instance.graph[u].degree() < instance.graph[v].degree();
    });
    for(int u = instance.num_vertexes - 1; u >= 0; u--) {
        for(const Vertex& v : instance.graph[u].neighbors()) {
            if(seen[v.id()] and not in_vc[v.id()]) {
                in_vc[u] = true;
                break;
            }
        }
        seen[u] = true;
    }
    return accumulate(in_vc.begin(), in_vc.end(), int(0));
}

// O(V^4) - caso patológico, na prática deve rodar muito mais rápido
int local_search_ma(const InstanceInfo& instance, vector<bool> in_vc) {
    int cur_vc_sz = accumulate(in_vc.begin(), in_vc.end(), int(0));
    for(int v = 0; v < static_cast<int>(instance.num_vertexes); v++) {
        if(not in_vc[v]) {
            bool can_replace = true;
            for(const Vertex& u : instance.graph[v].neighbors()) {
                if(not in_vc[u.id()]) {
                    can_replace = false;
                    break;
                }
            }
            if(can_replace) {
                vector<bool> new_in_vc(in_vc);
                new_in_vc[v] = true;
                for(const Vertex& u : instance.graph[v].neighbors()) {
                    bool still_needed = false;
                    for(const Vertex& w : instance.graph[u.id()].neighbors()) 
                        if(not new_in_vc[w.id()])
                        still_needed = true;
                    new_in_vc[u.id()] = still_needed;
                }
                if(accumulate(new_in_vc.begin(), new_in_vc.end(), int(0)) < cur_vc_sz)
                    return local_search_ma(instance, new_in_vc);
            }
        }
    }
    return cur_vc_sz;
}

// O(V^2.log(V) + local_search_ma)
int grasp_deg(const InstanceInfo& instance, double alpha) {
    int to_cover = instance.num_edges;
    vector<bool> in_vc(instance.num_vertexes, false);
    vector<int> deg(instance.num_vertexes);
    for(int v = 0; v < static_cast<int>(instance.num_vertexes); v++)
        deg[v] = instance.graph[v].degree();
    // O(V^2.log_2(V)))
    while(to_cover > 0) {
        vector<int> L;
        for(int v = 0; v < static_cast<int>(instance.num_vertexes); v++)
            if(not in_vc[v] and deg[v] > 0)
                L.emplace_back(v);
        stable_sort(L.rbegin(), L.rend(), [&] (int u, int v) {
            return deg[u] < deg[v];
        });
        int c_min = deg[L.back()], c_max = deg[L.front()], l = 0;
        // Já que estamos maximizando deg, alpha = 1 é guloso e 0 é totalmente aleatório
        while(l < static_cast<int>(L.size()) and deg[L[l]] >= c_min + alpha*(c_max - c_min) - epsilon)
            l++;
        assert(l > 0);
        int v = L[uniform(0, l - 1)];
        for(const Vertex& u : instance.graph[v].neighbors())
            if(not in_vc[u.id()])
                to_cover--, deg[u.id()]--, deg[v]--;
        assert(deg[v] == 0);
        in_vc[v] = true;
    }

    return local_search_ma(instance, in_vc);
}

// O(V^3 + local_search_ma)
int grasp_weideg(const InstanceInfo& instance, double alpha) {
    int to_cover = instance.num_edges;
    vector<bool> in_vc(instance.num_vertexes, false);
    vector<int> deg(instance.num_vertexes);
    for(int v = 0; v < static_cast<int>(instance.num_vertexes); v++)
        deg[v] = instance.graph[v].degree();
    // O(V^3)
    while(to_cover > 0) {
        vector<int> L;
        vector<double> weideg(instance.num_vertexes);
        for(int v = 0; v < static_cast<int>(instance.num_vertexes); v++) {
            if(not in_vc[v] and deg[v] > 0) {
                L.emplace_back(v);
                weideg[v] = deg[v];
                double sum_neighbors_deg = 0;
                for(const Vertex& u : instance.graph[v].neighbors())
                    sum_neighbors_deg += deg[u.id()];
                weideg[v] /= sum_neighbors_deg;
            }
        }
        stable_sort(L.rbegin(), L.rend(), [&] (int u, int v) {
            return weideg[u] != weideg[v] ? weideg[u] < weideg[v] : deg[u] < deg[v];
        });
        double c_min = weideg[L.back()], c_max = weideg[L.front()];
        int l = 0;
        // Já que estamos maximizando deg, alpha = 1 é guloso e 0 é totalmente aleatório
        while(l < static_cast<int>(L.size()) and weideg[L[l]] >= c_min + alpha*(c_max - c_min) - epsilon)
            l++;
        assert(l > 0);
        int v = L[uniform(0, l - 1)];
        for(const Vertex& u : instance.graph[v].neighbors())
            if(not in_vc[u.id()])
                to_cover--, deg[u.id()]--, deg[v]--;
        assert(deg[v] == 0);
        in_vc[v] = true;
    }

    return local_search_ma(instance, in_vc);
}

int main(int argc, char* argv[]) {
    vector<double> results;
    try {
        Arguments args = parse_arguments(argc, argv);
        validate_arguments(args);
        InstanceInfo instance = parse_input_file(args.input_path);
        switch(args.algorithm) {
            case Strategies::list_right: results = { static_cast<double>(lr(instance)) }; break;
            case Strategies::grasp_deg: results = sample_results(args.reps, grasp_deg, instance, args.alpha); break;
            case Strategies::grasp_weideg: results = sample_results(args.reps, grasp_weideg, instance, args.alpha); break;
            default: results = {};
        }
        print_results(args, instance, results);
    }catch(std::exception& e) {
        cout << "Erro: " << e.what() << endl;
    }
    return 0;
}
