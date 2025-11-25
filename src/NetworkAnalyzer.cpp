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

void NetworkAnalyzer::exportGraphToCSV(const Graph& g, string suffix, string folderPath) {
    string filename = folderPath + "/grafo_visual_" + suffix + ".csv";
    ofstream file(filename);

    if (!file.is_open()) return;

    file << "lat1,lon1,lat2,lon2,weight,type\n";

    // Cria checklist da MST
    set<pair<int, int>> mstLookup;
    for (const auto& e : mstEdges) mstLookup.insert({min(e.u, e.v), max(e.u, e.v)});

    for (const auto& edge : existingEdges) {
        int u = edge.first;
        int v = edge.second;
        const Station& s1 = stations[u];
        const Station& s2 = stations[v];
        double dist = haversine(s1, s2);

        string type = (mstLookup.count({u, v})) ? "MST" : "New_Connection";
        
        file << fixed << setprecision(6) 
             << s1.lat << "," << s1.lon << "," 
             << s2.lat << "," << s2.lon << "," 
             << dist << "," << type << "\n";
    }
    file.close();
    // cout << "   -> Visualizacao salva: " << filename << endl;
}

void NetworkAnalyzer::exportGraphToCSV(const Graph& g, int k, string folderPath) {
    exportGraphToCSV(g, "k" + to_string(k), folderPath);
}

void NetworkAnalyzer::runAnalysis(const vector<int>& k_values) {
    cout << "\n=== INICIANDO ANALISE DE CENARIOS ===" << endl;
    
    // Custo Base (da MST)
    double mstTotalKm = 0;
    for (const auto& e : mstEdges) mstTotalKm += e.weight;
    
    cout << "Custo Base: " << fixed << setprecision(2) << mstTotalKm << " km de tuneis.\n" << endl;

    // Garante que a pasta existe
    system("mkdir -p arquivos_relacionados/saidas_cpp");

    string summaryPath = "arquivos_relacionados/resumo_metricas_k.csv";
    ofstream summaryFile(summaryPath);
    summaryFile << "K,Custo_Bilhoes,Eficiencia_Km_Viagem\n"; 

    for (int k : k_values) {
        existingEdges.clear();
        Graph g(stations.size());

        // Reconstrói a Base (MST)
        for (const auto& e : mstEdges) {
            g.addEdge(e.u, e.v, e.weight);
            existingEdges.insert({min(e.u, e.v), max(e.u, e.v)});
        }

        double extraKm = addKNearestEdges(g, k);
        double totalKm = mstTotalKm + extraKm;
        
        // Custo: $200 Milhões por Km
        double totalCostBillions = (totalKm * 200.0) / 1000.0; 
        
        cout << "Analisando K=" << k << "..." << endl;
        double avgPath = g.calculateAveragePath();
        
        cout << "   | Custo Total: $" << totalCostBillions << " Bilhoes (" << totalKm << " km)" << endl;
        cout << "   | Tempo Medio (Eficiencia): " << avgPath << " km / viagem" << endl;

        summaryFile << k << "," << totalCostBillions << "," << avgPath << "\n";

        exportGraphToCSV(g, k, "arquivos_relacionados/saidas_cpp");
        cout << "--------------------------------------------" << endl;
    }

    summaryFile.close();
    cout << "Resumo das metricas salvo em: " << summaryPath << endl;
}

void NetworkAnalyzer::runSmartAnalysis(const vector<double>& factors) {
    cout << "\n=== INICIANDO BATERIA DE TESTES SMART (X) ===" << endl;
    
    string csvPath = "arquivos_relacionados/resumo_metricas_x.csv";
    ofstream xFile(csvPath);
    xFile << "Fator,Custo_Bilhoes,Eficiencia_Km_Viagem\n";

    for (double factor : factors) {
        cout << "Testando Fator de Alongamento: " << factor << "x ..." << endl;

        // Limpeza e MST Base
        existingEdges.clear();
        Graph g(stations.size());
        
        double mstCost = 0;
        for (const auto& e : mstEdges) {
            g.addEdge(e.u, e.v, e.weight);
            existingEdges.insert({min(e.u, e.v), max(e.u, e.v)});
            mstCost += e.weight;
        }

        // Roda o algoritmo pesado
        vector<Edge> smartEdges = g.optimizeByStretchFactor(stations, factor);

        // Atualiza visualização
        double extraKm = 0;
        for (const auto& e : smartEdges) {
            existingEdges.insert({min(e.u, e.v), max(e.u, e.v)});
            extraKm += e.weight;
        }

        // Métricas
        double totalKm = mstCost + extraKm;
        double totalCostB = (totalKm * 200.0) / 1000.0;
        double avgPath = g.calculateAveragePath();

        cout << "   | Novos Tuneis: " << smartEdges.size() 
             << " | Custo: $" << totalCostB << "B" 
             << " | Efic.: " << avgPath << "km" << endl;

        // Salva no CSV específico
        xFile << factor << "," << totalCostB << "," << avgPath << "\n";

        string label = "smart_" + to_string(factor).substr(0, 3); 
        exportGraphToCSV(g, label, "arquivos_relacionados/saidas_cpp");
    }

    xFile.close();
    cout << "Resumo Smart salvo em: " << csvPath << endl;
    cout << "--------------------------------------------" << endl;
}