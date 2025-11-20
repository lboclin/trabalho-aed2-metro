/**
 * Trabalho Final de Algoritmos e Estruturas de Dados 2
 * Tema: Otimização de Redes de Metrô usando MST e K-Nearest Neighbors
 * * Integrantes do Grupo:
 * - LUCAS BOCLIN
 * - MURILO HONORATO
 * - RAFAEL AUGUTSTO
 */

#include <iostream>
#include <vector>
#include <exception>
#include "NetworkAnalyzer.h"

using namespace std;

int main() {
    try {
        cout << "===================================================" << endl;
        cout << "   OTIMIZACAO DE REDE DE METRO - NY SUBWAY ANALYZER" << endl;
        cout << "===================================================" << endl;
    
        // Carregamento de Dados
        NetworkAnalyzer analyzer;
        string dataPath = "dados/MTA_Subway_Stations.csv";
        analyzer.loadData(dataPath);
    
        // Definicao de Cenarios
        int num_k;
        cout << "Digite a quantidade de K's que serão testados: ";
        cin >> num_k;
        cout << endl;
    
        vector<int> k_scenarios(num_k);
        cout << "Digite quais valores de K serão testados: ";
        for (int i = 0; i < num_k; i++) cin >> k_scenarios[i];
        cout << endl;
    
        // Execucao da Analise
        analyzer.runAnalysis(k_scenarios);
    
        cout << "\n===================================================" << endl;
        cout << "   ANALISE CONCLUIDA COM SUCESSO!" << endl;
        cout << "   Verifique a pasta 'arquivos_relacionados/saidas_cpp'" << endl;
        cout << "   para ver os arquivos CSV gerados para visualizacao." << endl;
        cout << "===================================================" << endl;
    } catch (const exception& e) {
        cerr << "ERRO FATAL: " << e.what() << endl;
        return 1;
    }

    return 0;
}