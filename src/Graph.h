#ifndef GRAPH_H
#define GRAPH_H

#include "DisjointSet.h"
#include "Utils.h"
#include <vector>

struct Edge {
    int u, v;
    double weight;

    // Verifica se esta aresta é menor do que a aresta comparada
    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

class Graph {
    private:
        int numNodes;
        std::vector<std::vector<std::pair<int, double>>> adjList;
    
    public:
        // Construtor: inicializa o grafo com 'n' nos
        Graph(int n);

        // Adiciona aresta
        void addEdge(int u, int v, double weight);

        // Calcula a MST (Kruskal)
        std::vector<Edge> kruskalMST(const std::vector<Station>& stations);

        // Calcula o caminho minimo a partir de um no
        std::vector<double> dijkstra(int startNode);

        // Metrica: calcula distancia media entre 2 pontos quaisquer do grafo
        double calculateAveragePath();

        // Calcula o caminho otimizado com Spanner Geométrico
        std::vector<Edge> optimizeByStretchFactor(const std::vector<Station>& stations, double factor);
};


#endif // GRAPH_H