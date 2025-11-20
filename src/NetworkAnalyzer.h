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
        double addKNearestEdges(Graph& g, int k);
        void exportGraphToCSV(const Graph& g, int k, std::string folderPath);
    
    public:
        NetworkAnalyzer();
        void loadData(const std::string& filename);
        void runAnalysis(const std::vector<int>& k_values);
};

#endif // NETWORK_ANALYZER_H