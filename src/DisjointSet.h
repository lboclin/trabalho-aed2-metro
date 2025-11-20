#ifndef DISJOINT_SET_H
#define DISJOINT_SET_H

#include <vector>

class DisjointSet {
    private:
        std::vector<int> rank;
        std::vector<int> parent;
        int numSets;

    public:
        // Funcao para iniciar o DSU
        DisjointSet(int n);

        // Funcao que retorna o pai do nó
        int find(int i);

        // Funcao que une 2 nós retornando true se ocorrer a uniao e false se nao ocorrer
        bool unite(int i, int j);
};

#endif // DISJOINT_SET_H