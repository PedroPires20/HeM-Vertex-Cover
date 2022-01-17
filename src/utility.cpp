#include <stdexcept>
#include <cstring>
#include <fstream>
#include "../include/utility.h"

using namespace std::string_literals;
using std::ifstream;
using std::getline;

inline bool is_switch(const char* str) {
    return (strlen(str) == 2)? str[0] == '-': false;
}

Arguments parse_arguments(int argc, char** argv) {
    Arguments args;
    if(argc < MIN_EXPECTED_ARGS + 1) 
        throw std::runtime_error("O número de argumentos fornecidos não é suficiente.");
    for(int i = 1; i < argc; i++) {
        if(is_switch(argv[i])) {
            if(i + 1 >= argc)
                throw std::runtime_error("O número de argumentos fornecidos não é suficiente.");
            switch(argv[i][1]) {
                case 'i': args.input_path = argv[i + 1]; break;
                case 's': args.strategy_name = argv[i + 1]; break;
                case 'a': args.alpha = atof(argv[i + 1]); break;
                default: throw std::runtime_error("Modificador "s + argv[i] + " não reconhecido.");
            }
            i++;
        }else {
            throw std::runtime_error("O formato dos argumentos fornecidos é incorreto.");
        }
    }
    return args;
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