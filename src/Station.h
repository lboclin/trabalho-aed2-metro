#ifndef STATION_H
#define STATION_H

#include <string>
#include <iostream>

struct Station {
    // Dados gerais da estacao
    int id;
    std::string originalId;
    std::string name;
    double lat; // latitude
    double lon; // longitude

    // Funcao de print para debug
    void print() const;
};


#endif // STATION_H