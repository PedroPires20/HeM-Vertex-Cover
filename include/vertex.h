#ifndef VERTEX_H
#define VERTEX_H
#include <string>
#include <list>

using std::string;
using std::list;

struct Edge {
    Edge(int dest_id, double wgt) : destination_id(dest_id), weight(wgt) {

    }

    int destination_id;
    double weight;
};

class Vertex {
public:
    Vertex(int id, const list<Edge>& adj_list = {}, const string& label = "");
    const int id() const;
    const list<Edge>& neighbors() const;
    const string& label() const;
    void relabel(const string& new_label);
    void add_neighboor(int vertex_id, double edge_weight);
    void remove_neighboor(int vertex_id);
    void clear_neighboors();
    double edge_cost(int neighbor_id) const;
    int degree() const;
private:
    int id_;
    string label_;
    list<Edge> adj_list_;
};

#endif