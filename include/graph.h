#ifndef GRAPH_H
#define GRAPH_H
#include <vector>
#include <tuple>
#include <exception>
#include <cmath>
#include "vertex.h"

using std::vector;
using std::tuple;

class VertexNotFoundException : public std::exception {
    const char* message_;
public:
    VertexNotFoundException(const char* message);
    VertexNotFoundException(const string& message);
    const char* what() const noexcept override;
};

/*
    A classe Graph representa um grafo não-direcionado ponderado por meio de uma 
    lista de adjacência. É armazenada uma lista de vértices (objetos da classe 
    "Vertex") e cada vértice possui uma lista das arestas que incidem sobre ele.
    Para cada aresta, é armazenado o índice do vértice de destino e o seu custo
    (representando por um número real).
*/
class Graph {
public:
    // Cria um novo grafo a partir da lista de vértices dada (por padrão, vazia)
    Graph(const vector<Vertex>& vertex_list = {});
    // Adiciona um vértice no grafo com os rótulos e vizinhos dados
    void add_vertex(const string& label = "", const list<Edge>& neighbors = {});
    // Remove o vértice cujo identificador é dado como argumento do grafo
    void remove_vertex(int vertex_id);
    // Retorna um objeto do tipo "Vertex" representando o vértice cujo identificador é dado
    const Vertex& vertex_at(int id) const;
    /*
        Adiciona uma aresta não direcionada com peso "weight" entre os vértices de índices 
        "origin_id" e "destination_id"
    */
    void add_edge(int origin_id, int destination_id, double weight);
    // Remove a aresta entre os vértices cujos identificadores são "origin_id" e "destination_id"
    void remove_edge(int origin_id, int destination_id);
    /*
        Retorna um vector representando a lista de arestas no grafo com tuplas 
        (tipo std::tuple) na forma (id_origem, id_destino, peso)
    */
    vector<tuple<int, int, double>> edge_list() const;
    // Retorna o custo da aresta entre os vértices cujos identificadores são "origin_id" e "destination_id"
    double edge_cost(int origin_id, int destination_id) const;
    // Retorna um vector com objetos da classe Vertex, representando os vértices no grafo
    const vector<Vertex>& vertex_list() const;
    /*
        Retorna a matriz de adjacência que representa o grafo. Cada entrada A[i,j] contém o peso
        da aresta entre i e j, se essa aresta existir. Caso a aresta não exista, seu peso é dado
        pela constante INFINITY, representando um custo "infinito" entre esses vértices. 
    */
    vector<vector<double>> adjacency_matrix() const;
    // Encontra o índice do vértice cujo rótulo é dado fazendo uma busca exaustiva nos vértices do grafo
    int find_vertex(const string& label) const;
    // Retorna o número de vértices do grafo
    int num_vertexes() const;
    // Retorna o número de arestas do grafo
    int num_edges() const;
    // Retorna um grafo representando a árvore geradora de custo mínimo para o grafo atual
    Graph minimum_spanning_tree() const;
    // Realiza uma busca em profundidade (DFS) no grafo a partir do vértice cujo identificador é dado como origem
    vector<int> dfs(int source_id) const;
    /*
        Esse enum enumera as estratégias (heurísticas) disponíveis para resolver instâncias do problema do 
        caixeiro viajante (TSP) nessa implementação da classe grafo.
    */
    enum class TSPStrategies {nearest_neighbor, twice_around_tree, VND, GRASP};
    /*
        Encontra uma solução de baixo custo para a instância do problema do caixeiro viajante (TSP) representada
        pelo grafo atual usando a estratégia dada por argumento (por padrão, uma heurística construtiva baseada na
        árvore geradora mínima do grafo atual). Esse método retorna o custo da solução obtida. Um ponteiro para um
        vector de inteiros pode ser opcionalmente dado como argumento para obter o ciclo hamiltoniano encontrado na
        forma de uma sequência de identificadores dos vértices do grafo. Para o GRASP, podem ser informados o número
        máximo de iterações (max_iter), o valor do parâmetro alpha e a tolerância para o filtro de qualidade (tol).
    */
    double tsp_tour(TSPStrategies strategy = TSPStrategies::twice_around_tree, vector<int>* tour_indexes = nullptr, double alpha = 0, double tol = INFINITY, int max_iter = 100) const;
    // Calcula o custo de uma solução ("tour") do TSP no grafo atual
    double tour_cost(const vector<int>& tour) const;
private:
    // Esse método implementa a heurística da árvore geradora mínima para o TSP
    double twice_around_tree(vector<int>* tour_indexes) const;
    // Esse método implementa a heurística do vizinho mais próximo para o TSP
    double greedy_tsp_tour(vector<int>* tour_indexes) const;
    double vnd_tsp_tour(vector<int>* tour_indexes) const;
    double random_greedy_tsp_tour(double tol, vector<int>* tour_indexes) const;
    double grasp_tsp_tour(double alpha, double tol, int max_iter, vector<int>* tour_indexes) const;

    vector<Vertex> vertexes_;
};

#endif