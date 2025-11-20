#ifndef DATA_LOADER_H
#define DATA_LOADER_H

#include "Station.h"
#include <vector>
#include <string>

// Funcao que transforma o dataset em um vetor de structs (estacoes)
std::vector<Station> loadFromFile(const std::string& filename);

#endif // DATA_LOADER_H