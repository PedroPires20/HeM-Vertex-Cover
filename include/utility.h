#ifndef UTILITY_H
#define UTILITY_H
#include <string>
#include "graph.h"
#define MIN_EXPECTED_ARGS 2

using std::string;


struct Arguments {
    string input_path, strategy_name;
    double alpha;
};

Arguments parse_arguments(int argc, char** argv);

struct InstanceInfo {
    size_t num_vertexes, num_edges;
    Graph graph;
};

InstanceInfo parse_input_file(const string& file_path);


#endif