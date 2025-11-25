#include "Graph.h"
#include "DisjointSet.h"
#include "Utils.h"
#include <algorithm>
#include <queue>
#include <limits>
#include <iostream>

Graph::Graph(int n) {
    adjList.resize(n);
    this->numNodes = n;
}

void Graph::addEdge(int u, int v, double weight) {
    adjList[u].push_back({v, weight});
    adjList[v].push_back({u, weight});
}

std::vector<Edge> Graph::kruskalMST(const std::vector<Station>& stations) {
    std::vector<Edge> allPossibleEdges;
    int N = stations.size();
    for (int i = 0; i < (N-1); i++) {
        for (int j = i+1; j < N; j++) {
            Edge newEdge;
            newEdge.u = stations[i].id;
            newEdge.v = stations[j].id;
            newEdge.weight = haversine(stations[i], stations[j]);
            allPossibleEdges.push_back(newEdge);
        }
    }

    std::sort(allPossibleEdges.begin(), allPossibleEdges.end());

    DisjointSet dsu(N);

    std::vector<Edge> mstEdges;
    
    for (const auto& edge : allPossibleEdges) {
        int u = edge.u;
        int v = edge.v;

        if (dsu.unite(u, v)) {
            mstEdges.push_back(edge);

            if (mstEdges.size() == N-1) {
                break;
            }
        }
    }

    return mstEdges;
}

std::vector<double> Graph::dijkstra(int startNode) {
    double inf = std::numeric_limits<double>::infinity();
    std::vector<double> dist (adjList.size(), inf);
    dist[startNode] = 0.0;

    std::priority_queue<std::pair<double, int>> pq;
    pq.push({-dist[startNode], startNode});

    while (pq.size() != 0) {
        double distA = -pq.top().first;
        int nodeA = pq.top().second;
        pq.pop();

        if (distA <= dist[nodeA]) {
            for (auto &edge : adjList[nodeA]) {
                double weight = edge.second;
                double nodeB = edge.first;
    
                if (distA + weight < dist[nodeB]) {
                    pq.push({-(distA+weight), nodeB});
                    dist[nodeB] = distA+weight;
                }
            }
        }
    }

    return dist;
}

double Graph::calculateAveragePath() {
    double totalDistanceSum = 0.0;
    long long validPathsCount = 0;
    double inf = std::numeric_limits<double>::infinity();

    for (int i = 0; i < numNodes; ++i) {
        std::vector<double> dist = dijkstra(i);

        for (int j = 0; j < numNodes; ++j) {
            if (i == j) continue;
            if (dist[j] == inf) continue;

            totalDistanceSum += dist[j];
            validPathsCount++;
        }
    }

    if (validPathsCount == 0) return 0.0;
    
    return totalDistanceSum / validPathsCount;
}

std::vector<Edge> Graph::optimizeByStretchFactor(const std::vector<Station>& stations, double factor) {
    std::vector<Edge> newEdges;
    double inf = std::numeric_limits<double>::infinity();
    int edgesCount = 0;

    std::cout << "   [Smart Algo] Iniciando otimizacao com Recalculo Dinamico (Fator " << factor << "x)..." << std::endl;

    for (int u = 0; u < numNodes; ++u) {
        std::vector<double> dists = dijkstra(u);

        for (int v = u + 1; v < numNodes; ++v) {
            
            double realDist = haversine(stations[u], stations[v]);
            double currentNetworkDist = dists[v];

            if (currentNetworkDist == inf || currentNetworkDist > (realDist * factor)) {
                addEdge(u, v, realDist);
                newEdges.push_back({u, v, realDist});
                edgesCount++;
                dists = dijkstra(u); 
            }
        }
        
        // if (u % 50 == 0) std::cout << "     -> Analisado no " << u << " (" << edgesCount << " tuneis criados ate agora)..." << std::endl;
    }

    return newEdges;
}