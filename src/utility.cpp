#include <stdexcept>
#include <cstring>
#include <fstream>
#include <cmath>
#include <iostream>
#include <algorithm>
#include "../include/utility.h"
#define MAX_REPS 100000

using namespace std::string_literals;
using std::ifstream, std::ofstream;
using std::getline;
using std::cout, std::endl;
using std::min_element;

inline bool is_switch(const char* str) {
    return (strlen(str) == 2)? str[0] == '-': false;
}

Strategies strategy_from_str(const char* str) {
    string input(str);
    for(char& c : input)
        c = tolower(c);
    if(input == "lr") {
        return Strategies::list_right;
    }else if(input == "gd") {
        return Strategies::grasp_deg;
    }else if(input == "gw") {
        return Strategies::grasp_weideg;
    }
    throw std::runtime_error("Estratégia \""s + str + "\" não reconhecida!\nAs estratégias (algoritmos) disponíveis são: List Right (lr), GRASP DEG (gd), GRASP WEIDEG (gw)");
}

const char* strategy_name(Strategies s) {
    switch(s) {
    case Strategies::list_right: return "List Right";
    case Strategies::grasp_deg: return "GRASP com heurística gulosa (DEG)";
    case Strategies::grasp_weideg: return "GRASP com heurística gulosa modificada (WEIDEG)";
    }
    return "";
} 

Arguments parse_arguments(int argc, char** argv) {
    Arguments args;
    string instance_name;
    // Definindo valores padrão para os argumentos alpha e reps
    args.alpha = 0;
    args.reps = 1;
    if(argc < MIN_EXPECTED_ARGS + 1) 
        throw std::runtime_error("O número de argumentos fornecidos não é suficiente.");
    for(int i = 1; i < argc; i++) {
        if(is_switch(argv[i])) {
            if(i + 1 >= argc)
                throw std::runtime_error("O número de argumentos fornecidos não é suficiente.");
            switch(argv[i][1]) {
                case 'i': args.input_path = argv[i + 1]; break;
                case 's': args.algorithm = strategy_from_str(argv[i + 1]); break;
                case 'a': args.alpha = atof(argv[i + 1]); break;
                case 'r': args.reps = atoi(argv[i + 1]); break;
                default: throw std::runtime_error("Modificador "s + argv[i] + " não reconhecido.");
            }
            i++;
        }else {
            throw std::runtime_error("O formato dos argumentos fornecidos é incorreto.");
        }
    }
    instance_name = args.input_path.substr(args.input_path.find_last_of("/\\") + 1);
    instance_name = instance_name.substr(0, instance_name.find_first_of('.'));
    args.instance_name = instance_name;
    return args;
}

void validate_arguments(const Arguments& args) {
    if(args.input_path == "")
        throw std::runtime_error("Nenhum caminho foi fornecido para o arquivo da instância a ser resolvida.");
    if(args.algorithm != Strategies::list_right) {
        if(args.alpha < 0 || args.alpha > 1)
            throw std::runtime_error("O valor fornecido para o parâmetro alpha ("s + std::to_string(args.alpha) + ") é inválido.\nO parâmetro alpha deve estar no intervalo [0,1].");
        if(args.reps <= 0 || args.reps > MAX_REPS)
            throw std::runtime_error("O número de repetições fornecido ("s + std::to_string(args.reps) + ") é inválido.\nO número de repetições deve ser positivo e não exceder "s + std::to_string(MAX_REPS) + ".");
    }
}

string trim(const string& s) {
    size_t start, end;
    const string white_spaces = " \t\n\v\f\r";
    start = s.find_first_not_of(white_spaces);
    end = s.find_last_not_of(white_spaces);
    if(start == string::npos || end == string::npos) {
        return s;
    }
    return s.substr(start, end - start + 1);
}

vector<string> split(const string& s, const char delimiter) {
    size_t delim_pos, last_pos = -1;
    vector<string> splits;
    string current_split;
    while((delim_pos = s.find(delimiter, last_pos + 1)) != string::npos) {
        current_split = s.substr(last_pos + 1, delim_pos - last_pos - 1);
        if(current_split.length() > 0) 
            splits.push_back(current_split);
        last_pos = delim_pos;
    }
    current_split = s.substr(last_pos + 1, s.length());
    if(current_split.length() > 0)
        splits.push_back(current_split);
    return splits;
}

InstanceInfo parse_input_file(const string& file_path) {
    ifstream input_file;
    string input_line;
    vector<string> line_results;
    bool graph_initialized = false;
    InstanceInfo data;
    try {
        input_file.open(file_path);
        if(!input_file.is_open())
            throw std::runtime_error(strerror(errno));
    }catch(std::exception& e) {
        throw std::runtime_error("Não foi possível abrir o arquivo de entrada.\nErro: "s + e.what());
    }
    while(getline(input_file, input_line)) {
        const string trimmed_line = trim(input_line);
        if(trimmed_line.size() == 0)
            throw std::runtime_error("O arquivo de entrada fornecido não segue o formato esperado!");
        if(trimmed_line[0] == 'c') {
            continue;
        }else if(trimmed_line[0] == 'p') {
            if(graph_initialized)
                throw std::runtime_error("O arquivo de entrada fornecido não segue o formato esperado!");
            line_results = split(trimmed_line, ' ');
            if(line_results.size() < 4)
                throw std::runtime_error("O arquivo de entrada fornecido não segue o formato esperado!");
            if(trim(line_results[1]) != "edge" && trim(line_results[1]) != "col")
                throw std::runtime_error("O arquivo de entrada fornecido não segue o formato esperado!");
            try{
                data.num_vertexes = std::stoi(line_results[2]);
                data.num_edges = std::stoi(line_results[3]);
                data.graph = Graph(data.num_vertexes);
                graph_initialized = true;
            }catch(std::exception& e) {
                throw std::runtime_error("O arquivo de entrada fornecido não segue o formato esperado!");
            }
        }else if(trimmed_line[0] == 'e') {
            line_results = split(trimmed_line, ' ');
            if(line_results.size() < 3)
                throw std::runtime_error("O arquivo de entrada fornecido não segue o formato esperado!");
            try{
                data.graph.add_edge(std::stoi(line_results[1]) - 1, std::stoi(line_results[2]) - 1);
            }catch(std::invalid_argument& e) {
                throw std::runtime_error("O arquivo de entrada fornecido não segue o formato esperado!");
            }catch(std::exception& e) {
                throw std::runtime_error("Não foi possível adicionar a aresta {" + line_results[1] + ", " + line_results[2] + "} ao grafo.");
            }
        }else {
            throw std::runtime_error("O arquivo de entrada fornecido não segue o formato esperado!");
        }
    }
    input_file.close();
    return data;
}

void export_graph_edgelist(const string& output_file_path, const Graph& g) {
    ofstream output_file;
    try{
        output_file.open(output_file_path);
        if(!output_file.is_open())
            throw std::runtime_error(strerror(errno));
    }catch(std::exception& e) {
        throw std::runtime_error("Erro ao escrever o arquivo de saída: "s + e.what());
    }
    for(const Vertex& v : g.vertex_list()) 
        for(const int j : v.neighbors())
            if(j > v.id())
                output_file << v.id() << " " << j << std::endl;
    output_file.close();
}

double mean(const vector<double>& data) {
    double sum = 0;
    for(const double& d : data)
        sum += d;
    return sum / data.size();
}

double deviation(const vector<double>& data) {
    double mu = mean(data), var = 0;
    for(const double& d : data)
        var += (d - mu) * (d - mu);
    var /= data.size();
    return sqrt(var);
}

void print_results(const Arguments& args, const InstanceInfo& instance, const vector<double>& results) {
    const double mu = mean(results), sigma = deviation(results);
    const bool is_random = args.algorithm != Strategies::list_right;

    cout << "Nome da instância resolvida: " << args.instance_name << endl;
    cout << "Número de vértices: " << instance.num_vertexes << endl;
    cout << "Número de arestas: " << instance.num_edges << endl;
    cout << "Heurística utilizada: " << strategy_name(args.algorithm) << endl;
    if(is_random) {
        cout << "Valor do parâmetro (alpha) utilizado: " << args.alpha << endl;
        cout << "Número de execuções do algoritmo: " << args.reps << endl;
    }
    cout << "Tamanho da cobertura" << (is_random? " (média)": "") << " encontrada: " << mu << endl;
    if(is_random) {
        cout << "Tamanho da menor cobertura encontrada: " << *min_element(results.begin(), results.end()) << endl;
        cout << "Desvio padrão das coberturas encontradas: " << sigma << endl;
    }
}

vector<double> sample_results(int reps, int(*f)(const InstanceInfo&, double), const InstanceInfo& instance, double alpha) {
    vector<double> results;
    results.reserve(reps);
    for(int i = 0; i < reps; i++)
        results.emplace_back(static_cast<double>(f(instance, alpha)));
    return results;
}
