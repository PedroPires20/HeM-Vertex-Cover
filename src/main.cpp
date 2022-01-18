#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include "../include/graph.h"
#include "../include/utility.h"

using namespace std;

// Exemplo de função a ser utilizada para o LR
int lr(const InstanceInfo& instance) {
    return instance.num_vertexes / 2;
}

// Exemplo de funções a serem utilizada para implementar um GRASP
int grasp_nosso(const InstanceInfo& instance, double alpha) {
    return instance.num_edges / (rand() % 11 + 1);
}

int grasp(const InstanceInfo& instance, double alpha) {
    return instance.num_edges / (rand() % 21 + 1);
}

int main(int argc, char* argv[]) {
    vector<double> results;
    try {
        Arguments args = parse_arguments(argc, argv);
        validate_arguments(args);
        InstanceInfo instance = parse_input_file(args.input_path);
        switch(args.algorithm) {
            case Strategies::list_right: results = { static_cast<double>(lr(instance)) }; break;
            case Strategies::grasp_deg: results = sample_results(args.reps, grasp_nosso, instance, args.alpha); break;
            case Strategies::grasp_weideg: results = sample_results(args.reps, grasp, instance, args.alpha); break;
            default: results = {};
        }
        print_results(args, instance, results);
    }catch(std::exception& e) {
        cout << "Erro: " << e.what() << endl;
    }
    return 0;
}
