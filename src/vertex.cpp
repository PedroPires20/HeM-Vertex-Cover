#include "../include/vertex.h"
#include <cmath>

Vertex::Vertex(int id, const list<Edge>& adj_list, const string& label) : 
id_(id), label_(label), adj_list_(adj_list) {

}

const int Vertex::id() const {
    return id_;
}

const list<Edge>& Vertex::neighbors() const {
    return adj_list_;
}

const string& Vertex::label() const {
    return label_;
}

void Vertex::relabel(const string& new_label) {
    label_ = new_label;
}

void Vertex::add_neighboor(int vertex_id, double edge_weight) {
    adj_list_.emplace_back(vertex_id, edge_weight);
}

void Vertex::remove_neighboor(int vertex_id) {
    for(auto it = adj_list_.begin(); it != adj_list_.end(); it++) {
        if(it->destination_id == vertex_id) {
            adj_list_.erase(it);
            break;
        }
    }
}

void Vertex::clear_neighboors() {
    adj_list_.clear();
}

double Vertex::edge_cost(int neighbor_id) const {
    for(auto it = adj_list_.begin(); it != adj_list_.end(); it++) {
        if(it->destination_id == neighbor_id) {
            return it->weight;
        }
    }
    return INFINITY;
}

int Vertex::degree() const {
    return adj_list_.size();
}
