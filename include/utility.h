#ifndef UTILITY_H
#define UTILITY_H
#include <string>
#include "graph.h"
#define MIN_EXPECTED_ARGS 2

using std::string;

enum class Strategies { list_right, grasp_deg, grasp_weideg };

struct Arguments {
    string input_path;
    Strategies algorithm;
    double alpha;
    int reps;
};

Arguments parse_arguments(int argc, char** argv);

struct InstanceInfo {
    size_t num_vertexes, num_edges;
    Graph graph;
};

InstanceInfo parse_input_file(const string& file_path);

void export_graph_edgelist(const string& output_file_path, const Graph& g);

double mean(const vector<double>& data);

double deviation(const vector<double>& data);

#endif