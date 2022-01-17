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

void Graph::remove_vertex(int vertex_id) {
    if(vertex_id >= (int)vertexes_.size())
        throw VertexNotFoundException("Erro ao remover o vértice de índice " + to_string(vertex_id) + ": vértice não encontrado no grafo!");
    vertexes_.erase(vertexes_.begin() + vertex_id);
}

const Vertex& Graph::vertex_at(int id) const {
    if(id >= (int)vertexes_.size())
        throw VertexNotFoundException("Erro ao recuperar o vértice de índice" + to_string(id) + ": vértice não encontrado no grafo!");
    return vertexes_[id];
}

void Graph::add_edge(int origin_id, int destination_id) {
    if(origin_id >= (int)vertexes_.size())
        throw VertexNotFoundException("Erro ao adicionar a aresta (" + to_string(origin_id) + ", " + to_string(destination_id) + ": o vértice de origem não existe!");
    if(destination_id >= (int)vertexes_.size())
        throw VertexNotFoundException("Erro ao adicionar a aresta (" + to_string(origin_id) + ", " + to_string(destination_id) + ": o vértice de destino não existe!");
    vertexes_[origin_id].add_neighboor(destination_id);
    vertexes_[destination_id].add_neighboor(origin_id);
}

void Graph::remove_edge(int origin_id, int destination_id) {
    if(origin_id >= (int)vertexes_.size())
        throw VertexNotFoundException("Erro ao remover a aresta (" + to_string(origin_id) + ", " + to_string(destination_id) + ": o vértice de origem não existe!");
    if(destination_id >= (int)vertexes_.size())
        throw VertexNotFoundException("Erro ao remover a aresta (" + to_string(origin_id) + ", " + to_string(destination_id) + ": o vértice de destino não existe!");
    vertexes_[origin_id].remove_neighboor(destination_id);
    vertexes_[destination_id].remove_neighboor(origin_id);
}

vector<tuple<int, int>> Graph::edge_list() const {
    vector<tuple<int, int>> edges;
    for(size_t i = 0; i < vertexes_.size(); i++) {
        for(int j : vertexes_[i].neighbors()) {
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
