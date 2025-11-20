#include "DataLoader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

std::vector<Station> loadFromFile(const std::string& filename) {
    std::vector<Station> stations;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Erro: Não foi possível abrir o arquivo " << filename << "/n";
        return stations;
    }

    std::string line;
    std::getline(file, line);

    int indexCounter = 0;
    const int EXPECTED_COLUMNS = 12;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cell;
        std::vector<std::string> row;


        while (std::getline(ss, cell, ',')) {
            row.push_back(cell);
        }

        if (row.size() >= EXPECTED_COLUMNS) {
            Station s;
            s.id = indexCounter++;
            s.originalId = row[0];
            s.name = row[5];
            
            try {
                s.lat = stod(row[10]);
                s.lon = stod(row[11]);
                stations.push_back(s);
            } catch (const std::exception& e) {
                std::cerr << "Aviso: Linha corrompida. Erro: " << e.what() << std::endl;
                indexCounter--;
            }
        }
    }
    
    file.close();
    return stations;

}
