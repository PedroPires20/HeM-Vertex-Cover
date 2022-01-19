#ifndef GRAPH_H
#define GRAPH_H
#include <vector>
#include <string>
#include <tuple>
#include <exception>
#include <cmath>
#include "vertex.h"

using std::vector;
using std::string;
using std::tuple;

class VertexNotFoundException : public std::exception {
    const char* message_;
public:
    VertexNotFoundException(const char* message);
    VertexNotFoundException(const string& message);
    const char* what() const noexcept override;
};

/*
    A classe Graph representa um grafo não-direcionado por meio de uma 
    lista de adjacência. É armazenada uma lista de vértices (objetos da classe 
    "Vertex") e cada vértice possui uma com os IDs de seus vizinhos.
*/
class Graph {
public:
    // Cria um novo grafo com o número de vértices dado
    Graph(size_t num_vertexes);
    // Cria um novo grafo a partir da lista de vértices dada (por padrão, vazia)
    Graph(const vector<Vertex>& vertex_list = {});
    // Adiciona um vértice no grafo com os vizinhos dados. Retorna o id do vértice adicionado
    int add_vertex(const vector<int>& neighbors = {});
    // Retorna um objeto do tipo "Vertex" representando o vértice cujo identificador é dado
    const Vertex& vertex_at(size_t id) const;
    Vertex& vertex_at(size_t id);
    // Adiciona uma aresta não direcionada entre os vértices de índices "origin_id" e "destination_id"
    void add_edge(int origin_id, int destination_id);
    // Remove a aresta entre os vértices cujos identificadores são "origin_id" e "destination_id"
    void remove_edge(int origin_id, int destination_id);
    /*
        Retorna um vector representando a lista de arestas no grafo com tuplas 
        (tipo std::tuple) na forma (id_origem, id_destino)
    */
    vector<tuple<int, int>> edge_list() const;
    // Retorna um vector com objetos da classe Vertex, representando os vértices no grafo
    const vector<Vertex>& vertex_list() const;
    /*
        Retorna a matriz de adjacência que representa o grafo. Cada entrada A[i,j] é igual a 1
        se, e somente se, existe uma aretas entre i e j no grafo
    */
    vector<vector<bool>> adjacency_matrix() const;
    // Retorna o número de vértices do grafo
    int num_vertexes() const;
    // Retorna o número de arestas do grafo
    int num_edges() const;
    // Sobrecarregando o operador [] para retornar o vértice na posição dada
    inline Vertex& operator[](size_t pos) {
        return vertex_at(pos);
    }

    inline const Vertex& operator[](size_t pos) const {
        return vertex_at(pos);
    }
    // Retorna o complemento do grafo atual
    Graph complement() const;
private:

    vector<Vertex> vertexes_;
};

#endif