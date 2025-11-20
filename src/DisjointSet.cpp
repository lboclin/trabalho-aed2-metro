#include "DisjointSet.h"
#include <iostream>
#include <numeric>

DisjointSet::DisjointSet(int n) : numSets(n) {
    parent.resize(n);
    rank.resize(n, 0);

    for (int i = 0; i < n; i++) {
        parent[i] = i;
    }
}

int DisjointSet::find(int i) {
    if (parent[i] == i) {
        return i;
    }

    return parent[i] = find(parent[i]);
}

bool DisjointSet::unite(int i, int j) {
    i = find(i);
    j = find(j);

    if (i == j) {
        return false;
    }

    if (rank[i] < rank[j]) {
        std::swap(i, j);
    }

    parent[j] = i;

    if (rank[i] == rank[j]) { 
        rank[i]++;
    }
    
    numSets--;

    return true;
}