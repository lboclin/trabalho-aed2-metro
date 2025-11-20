#ifndef UTILS_H
#define UTILS_H

#include "Station.h"

// Funcao que retorna, em km, a distância entre duas estacoes
double haversine(const Station& A, const Station& B);

#endif // UTILS_H