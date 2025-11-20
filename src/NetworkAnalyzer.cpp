#include "NetworkAnalyzer.h"
#include "DataLoader.h"
#include "Utils.h"
#include "DisjointSet.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <filesystem>

using namespace std;

NetworkAnalyzer::NetworkAnalyzer() {
    // cout << "[System] NetworkAnalyzer inicializado." << endl;
}

void NetworkAnalyzer::loadData(const std::string& filename) {
    cout << "[Loader] Lendo arquivo: " << filename << "..." << endl;
    this->stations = loadFromFile(filename);
    cout << "[Loader] " << stations.size() << " estacoes carregadas." << endl;

    // Geracao da MST
    cout << "[MST] Gerando Árvore Geradora Minima..." << endl;

    Graph tempGraph(stations.size());
    this->mstEdges = tempGraph.kruskalMST(stations);

    cout << "MST criada com " << mstEdges.size() << " conexoes." << endl;
}

double NetworkAnalyzer::addKNearestEdges(Graph& g, int k) {
    double extraCostKm = 0.0;
    // int edgesAddedCount = 0;

    for (const auto& station : stations) {
        vector<pair<double, int>> candidates;
        candidates.reserve(stations.size());

        for (const auto& neighbor : stations) {
            if (station.id == neighbor.id) continue; 
            
            double dist = haversine(station, neighbor);
            candidates.push_back({dist, neighbor.id});
        }

        std::sort(candidates.begin(), candidates.end());

        int addedForThisStation = 0;
        for (const auto& cand : candidates) {
            if (addedForThisStation >= k) break;

            int u = station.id;
            int v = cand.second;
            double w = cand.first;

            pair<int, int> edgeKey = {min(u, v), max(u, v)};

            if (existingEdges.find(edgeKey) != existingEdges.end()) {
                continue; 
            }

            g.addEdge(u, v, w);
            existingEdges.insert(edgeKey);
            extraCostKm += w;
            
            addedForThisStation++;
            // edgesAddedCount++;
        }
    }
    
    return extraCostKm;
}

void NetworkAnalyzer::exportGraphToCSV(const Graph& g, int k, string folderPath) {
    string filename = folderPath + "/grafo_visual_k" + to_string(k) + ".csv";
    ofstream file(filename);

    if (!file.is_open()) {
        cerr << "Erro ao criar arquivo de visualizacao: " << filename << endl;
        return;
    }

    file << "lat1,lon1,lat2,lon2,weight,type\n";

    set<pair<int, int>> mstLookup;
    for (const auto& e : mstEdges) {
        mstLookup.insert({min(e.u, e.v), max(e.u, e.v)});
    }

    for (const auto& edge : existingEdges) {
        int u = edge.first;
        int v = edge.second;
        
        const Station& s1 = stations[u];
        const Station& s2 = stations[v];
        double dist = haversine(s1, s2);

        string type;
        
        if (mstLookup.find({u, v}) != mstLookup.end()) {
            type = "MST"; 
        } else {
            type = "New_Connection";
        }
        
        file << fixed << setprecision(6) 
             << s1.lat << "," << s1.lon << "," 
             << s2.lat << "," << s2.lon << "," 
             << dist << "," << type << "\n";
    }

    file.close();
    cout << "   -> Visualizacao salva em: " << filename << endl;
}

void NetworkAnalyzer::runAnalysis(const vector<int>& k_values) {
    cout << "\n=== INICIANDO ANALISE DE CENARIOS ===" << endl;
    
    // Custo Base (da MST)
    double mstTotalKm = 0;
    for (const auto& e : mstEdges) mstTotalKm += e.weight;
    
    cout << "Custo Base: " << fixed << setprecision(2) << mstTotalKm << " km de tuneis.\n" << endl;

    system("mkdir -p arquivos_relacionados/saidas_cpp");

    for (int k : k_values) {
        existingEdges.clear();
        Graph g(stations.size());

        for (const auto& e : mstEdges) {
            g.addEdge(e.u, e.v, e.weight);
            existingEdges.insert({min(e.u, e.v), max(e.u, e.v)});
        }

        double extraKm = addKNearestEdges(g, k);
        double totalKm = mstTotalKm + extraKm;
        
        // Custo: Vamos assumir $200 Milhões por Km
        double totalCost = totalKm * 200.0; 
        
        cout << "Analisando K=" << k << "..." << endl;
        double avgPath = g.calculateAveragePath();
        
        cout << "   | Custo Total: $" << (totalCost/1000.0) << " Bilhoes (" << totalKm << " km)" << endl;
        cout << "   | Tempo Medio (Eficiencia): " << avgPath << " km / viagem" << endl;

        exportGraphToCSV(g, k, "arquivos_relacionados/saidas_cpp");
        cout << "--------------------------------------------" << endl;
    }
}
