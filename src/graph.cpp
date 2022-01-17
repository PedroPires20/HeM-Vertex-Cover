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
        vertexes_.emplace_back(i, vertex_list[i].neighbors(), vertex_list[i].label());
    }
}

void Graph::add_vertex(const string& label, const list<Edge>& neighbors) {
    vertexes_.emplace_back(vertexes_.size(), neighbors, label);
}

void Graph::remove_vertex(int vertex_id) {
    if(vertex_id >= (int)vertexes_.size())
        throw VertexNotFoundException("Erro ao remover o vértice de índice " + to_string(vertex_id) + ": vértice não encontrado no grafo!");
    for(const Edge& e : vertexes_[vertex_id].neighbors()) {
        vertexes_[e.destination_id].remove_neighboor(vertex_id);
    }
    vertexes_.erase(vertexes_.begin() + vertex_id);
}

const Vertex& Graph::vertex_at(int id) const {
    if(id >= (int)vertexes_.size())
        throw VertexNotFoundException("Erro ao recuperar o vértice de índice" + to_string(id) + ": vértice não encontrado no grafo!");
    return vertexes_[id];
}

void Graph::add_edge(int origin_id, int destination_id, double weight) {
    if(origin_id >= (int)vertexes_.size())
        throw VertexNotFoundException("Erro ao adicionar a aresta (" + to_string(origin_id) + ", " + to_string(destination_id) + ": o vértice de origem não existe!");
    if(destination_id >= (int)vertexes_.size())
        throw VertexNotFoundException("Erro ao adicionar a aresta (" + to_string(origin_id) + ", " + to_string(destination_id) + ": o vértice de destino não existe!");
    vertexes_[origin_id].add_neighboor(destination_id, weight);
    vertexes_[destination_id].add_neighboor(origin_id, weight);
}

void Graph::remove_edge(int origin_id, int destination_id) {
    if(origin_id >= (int)vertexes_.size())
        throw VertexNotFoundException("Erro ao remover a aresta (" + to_string(origin_id) + ", " + to_string(destination_id) + ": o vértice de origem não existe!");
    if(destination_id >= (int)vertexes_.size())
        throw VertexNotFoundException("Erro ao remover a aresta (" + to_string(origin_id) + ", " + to_string(destination_id) + ": o vértice de destino não existe!");
    vertexes_[origin_id].remove_neighboor(destination_id);
    vertexes_[destination_id].remove_neighboor(origin_id);
}

vector<tuple<int, int, double>> Graph::edge_list() const {
    vector<tuple<int, int, double>> edges;
    for(size_t i = 0; i < vertexes_.size(); i++) {
        for(const Edge e : vertexes_[i].neighbors()) {
            edges.emplace_back(i, e.destination_id, e.weight);
        }
    }
    return edges;
}

double Graph::edge_cost(int origin_id, int destination_id) const {
    return vertexes_[origin_id].edge_cost(destination_id);
}

const vector<Vertex>& Graph::vertex_list() const {
    return vertexes_;
}

vector<vector<double>> Graph::adjacency_matrix() const {
    int num_vertexes = vertexes_.size();
    vector<vector<double>> adj = vector<vector<double>>(num_vertexes, vector<double>(num_vertexes, INFINITY));
    for(int i = 0; i < num_vertexes; i++) {
        for(const Edge& e : vertexes_[i].neighbors()) {
            adj[i][e.destination_id] = e.weight;
            adj[e.destination_id][i] = e.weight;
        }
    }
    return adj;
}

int Graph::find_vertex(const string& label) const {
    int vertex_index = -1;
    for(size_t i = 0; i < vertexes_.size(); i++) {
        if(vertexes_[i].label() == label) {
            vertex_index = i;
            break;
        }
    }
    if(vertex_index < 0)
        throw VertexNotFoundException("Erro na busca: vértice \"" + label + "\" não encontrado no grafo!");
    return vertex_index;
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
