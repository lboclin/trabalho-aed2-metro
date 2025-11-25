#ifndef NETWORK_ANALYZER_H
#define NETWORK_ANALYZER_H

#include <vector>
#include <string>
#include <set>
#include "Station.h"
#include "Graph.h"

class NetworkAnalyzer {
    private:
        std::vector<Station> stations;
        std::vector<Edge> mstEdges;
        std::set<std::pair<int, int>> existingEdges;

        // Adiciona k arestas para os k vizinhos mais proximos de cada estacao
        double addKNearestEdges(Graph& g, int k);

        // Exporta o grafo em csv para facilitar no plot
        void exportGraphToCSV(const Graph& g, int k, std::string folderPath);
        void exportGraphToCSV(const Graph& g, std::string suffix, std::string folderPath);
    
    public:
        NetworkAnalyzer();
        
        // Carrega o arquivo .csv e roda a MST
        void loadData(const std::string& filename);

        // Roda todos os codigos para a analise
        void runAnalysis(const std::vector<int>& k_values);

        void runSmartAnalysis(const std::vector<double>& factors);
};

#endif // NETWORK_ANALYZER_H