#include "../include/graph.h"
#include <stack>
#include <list>

using std::stack;
using std::list;
using std::to_string;
using std::tie;

VertexNotFoundException::VertexNotFoundException(const char* message) : message_(message) {

}

VertexNotFoundException::VertexNotFoundException(const string& message) : message_(message.c_str()) {
    
}

const char* VertexNotFoundException::what() const noexcept {
    return message_;
}

Graph::Graph(size_t num_vertexes) {
    vertexes_ = {};
    for(int i = 0; i < static_cast<long long>(num_vertexes); i++)
        vertexes_.emplace_back(i, vector<int>());
}

Graph::Graph(const vector<Vertex>& vertex_list) {
    int num_vertexes = vertex_list.size();
    vertexes_ = vector<Vertex>();
    for(int i = 0; i < num_vertexes; i++) {
        vertexes_.emplace_back(i, vertex_list[i].neighbors());
    }
}

int Graph::add_vertex(const vector<int>& neighbors) {
    vertexes_.emplace_back(vertexes_.size(), neighbors);
    return vertexes_.size() - 1;
}

Vertex& Graph::vertex_at(size_t id) {
    if(id >= vertexes_.size())
        throw VertexNotFoundException("Erro ao recuperar o vértice de índice" + to_string(id) + ": vértice não encontrado no grafo!");
    return vertexes_[id];
}

const Vertex& Graph::vertex_at(size_t id) const {
    if(id >= vertexes_.size())
        throw VertexNotFoundException("Erro ao recuperar o vértice de índice" + to_string(id) + ": vértice não encontrado no grafo!");
    return vertexes_[id];
}

void Graph::add_edge(int origin_id, int destination_id) {
    if(origin_id >= (int)vertexes_.size())
        throw VertexNotFoundException("Erro ao adicionar a aresta (" + to_string(origin_id) + ", " + to_string(destination_id) + ": o vértice de origem não existe!");
    if(destination_id >= (int)vertexes_.size())
        throw VertexNotFoundException("Erro ao adicionar a aresta (" + to_string(origin_id) + ", " + to_string(destination_id) + ": o vértice de destino não existe!");
    vertexes_[origin_id].add_neighbor(destination_id);
    vertexes_[destination_id].add_neighbor(origin_id);
}

void Graph::remove_edge(int origin_id, int destination_id) {
    if(origin_id >= (int)vertexes_.size())
        throw VertexNotFoundException("Erro ao remover a aresta (" + to_string(origin_id) + ", " + to_string(destination_id) + ": o vértice de origem não existe!");
    if(destination_id >= (int)vertexes_.size())
        throw VertexNotFoundException("Erro ao remover a aresta (" + to_string(origin_id) + ", " + to_string(destination_id) + ": o vértice de destino não existe!");
    vertexes_[origin_id].remove_neighbor(destination_id);
    vertexes_[destination_id].remove_neighbor(origin_id);
}

vector<tuple<int, int>> Graph::edge_list() const {
    vector<tuple<int, int>> edges;
    for(int i = 0; i < static_cast<long long>(vertexes_.size()); i++) {
        for(int j : vertexes_[i].neighbors()) {
            if(j > i)
                edges.emplace_back(i, j);
        }
    }
    return edges;
}

const vector<Vertex>& Graph::vertex_list() const {
    return vertexes_;
}

vector<vector<bool>> Graph::adjacency_matrix() const {
    int num_vertexes = vertexes_.size();
    vector<vector<bool>> adj = vector<vector<bool>>(num_vertexes, vector<bool>(num_vertexes, false));
    for(int i = 0; i < num_vertexes; i++) {
        for(int j : vertexes_[i].neighbors()) {
            adj[i][j] = true;
        }
    }
    return adj;
}

int Graph::num_vertexes() const {
    return vertexes_.size();
}

int Graph::num_edges() const {
    int n_edges = 0;
    for(const Vertex v : vertexes_) {
        n_edges += v.degree();
    }
    return n_edges / 2;
}

Graph Graph::complement() const  {
    size_t n = vertexes_.size();
    vector<bool> neighbors_complement(n, true);
    vector<int> c_neighbors;
    Graph complement;
    c_neighbors.reserve(n);
    for(size_t i = 0; i < n; i++) {
        neighbors_complement[i] = false;
        for(int j : vertexes_[i].neighbors())
            neighbors_complement[j] = false;
        for(size_t k = 0; k < n; k++)
            if(neighbors_complement[k])
                c_neighbors.emplace_back(k);
        complement.vertexes_.emplace_back(i, c_neighbors);
        c_neighbors.clear();
        neighbors_complement.assign(n, true);
    }
    return complement;
}
