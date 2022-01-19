#ifndef VERTEX_H
#define VERTEX_H
#include <vector>

using std::vector;

class Vertex {
public:
    inline Vertex(int id, const vector<int>& adj_list = {}) : id_(id), neighbors_(adj_list) {
        
    }
    
    inline const int id() const {
        return id_;
    }

    inline const vector<int>& neighbors() const {
        return neighbors_;
    }

    inline void add_neighbor(int vertex_id) {
        neighbors_.push_back(vertex_id);
    }

    void remove_neighbor(int vertex_id) {
        for(auto it = neighbors_.begin(); it != neighbors_.end(); it++) {
            if(*it == vertex_id)
                neighbors_.erase(it++);
        }
    }
    
    inline void clear_neighbors() {
        neighbors_.clear();
    }
    
    inline int degree() const {
        return neighbors_.size();
    }
private:
    int id_;
    vector<int> neighbors_;
};

#endif