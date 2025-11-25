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
        if (num_k != 0) {
            cout << "Digite quais valores de K serão testados: ";
        }
        for (int i = 0; i < num_k; i++) cin >> k_scenarios[i];
        cout << endl;

        double num_x;
        cout << "Digite a quantidade de X's que serão testados: ";
        cin >> num_x;
        cout << endl;

        vector<double> x_scenarios(num_x);
        if (num_x != 0) {
            cout << "Digite quais valores de X serão testados: ";
        }
        bool negative_value = false;
        for (int i = 0; i < num_x; i++) {
            cin >> x_scenarios[i];
            if (x_scenarios[i] < 0) negative_value = true;
        }
        cout << endl;
    
        // Execucao da Analise
        analyzer.runAnalysis(k_scenarios);
        if (negative_value) {
            cout << "Erro. Digite apenas valores positivos para X.\nO algoritmo inteligente não rodou." << endl;
        } else {
            analyzer.runSmartAnalysis(x_scenarios);
        }
        
    
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