#ifndef UTILITY_H
#define UTILITY_H
#include <string>
#include "graph.h"
#define MIN_EXPECTED_ARGS 2

using std::string;

enum class Strategies { list_right, grasp_deg, grasp_weideg };

struct Arguments {
    string input_path, instance_name;
    Strategies algorithm;
    double alpha;
    int reps;
};

Arguments parse_arguments(int argc, char** argv);

void validate_arguments(const Arguments& args);

struct InstanceInfo {
    size_t num_vertexes, num_edges;
    Graph graph;
};

InstanceInfo parse_input_file(const string& file_path);

void export_instance(const string& output_file_path, const InstanceInfo& instance);

double mean(const vector<double>& data);

double deviation(const vector<double>& data);

void print_results(const Arguments& args, const InstanceInfo& instance, const vector<double>& results);

/*
    Essa função amostra os resultados da execução do GRASP, executando a função (f) dada como parâmetro,
    que deve retornar um inteiro e receber, como parâmetro, os dados da instância e um real com o valor,
    do parâmetro alpha, isto é, a assinatura da função deve ser int f(const InstanceInfo&, double). A 
    função f é executada pelo número de vezes especificado pelo inteiro positivo (reps) e os resultados 
    retornados são convertidos para reais (double) e armazenados em um vetor, que é retornado como 
    resultado.
*/
vector<double> sample_results(int reps, int(*f)(const InstanceInfo&, double), const InstanceInfo& instance, double alpha);


#endif