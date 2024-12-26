#include "GraphOperations.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <climits>

using namespace std;

int** createG(int size) {
    int** G = new int* [size];
    for (int i = 0; i < size; i++) {
        G[i] = new int[size];
        for (int j = 0; j < size; j++) {
            G[i][j] = (i != j && rand() % 2 == 0) ? rand() % 7 + 1 : 0;
        }
    }
    return G;
}

void printG(int** G, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            cout << G[i][j] << " ";
        }
        cout << endl;
    }
}

bool isSafe(int v, int** G, vector<int>& path, int pos) {
    if (G[path[pos - 1]][v] == 0) return false;
    for (int i = 0; i < pos; i++) {
        if (path[i] == v) return false;
    }
    return true;
}

void hamiltonianCycleUtil(int** G, vector<int>& path, int pos, int size, vector<vector<int>>& allPaths) {
    if (pos == size) {
        if (G[path[pos - 1]][path[0]] > 0) {
            allPaths.push_back(path);
        }
        return;
    }

    for (int v = 1; v < size; v++) {
        if (isSafe(v, G, path, pos)) {
            path[pos] = v;
            hamiltonianCycleUtil(G, path, pos + 1, size, allPaths);
            path[pos] = -1;
        }
    }
}

int calculateCycleWeight(int** G, const vector<int>& path) {
    int weight = 0;
    for (int i = 0; i < path.size() - 1; i++) {
        weight += G[path[i]][path[i + 1]];
    }
    weight += G[path[path.size() - 1]][path[0]];
    return weight;
}

void findAllHamiltonianCycles(int** G, int size, int start) {
    vector<int> path(size, -1);
    path[0] = start;

    vector<vector<int>> allPaths;
    hamiltonianCycleUtil(G, path, 1, size, allPaths);

    if (allPaths.empty()) {
        cout << "Гамильтонов циклов не существует." << endl;
    }
    else {
        cout << "Найдено Гамильтонов циклов: " << allPaths.size() << endl;

        int minWeight = INT_MAX;
        vector<int> shortestCycle;

        for (size_t i = 0; i < allPaths.size(); i++) {
            vector<int>& cycle = allPaths[i];
            for (size_t j = 0; j < cycle.size(); j++) {
                cout << cycle[j] + 1 << " ";
            }
            cout << cycle[0] + 1 << endl;
            int weight = calculateCycleWeight(G, cycle);
            cout << "Вес цикла: " << weight << endl;

            if (weight < minWeight) {
                minWeight = weight;
                shortestCycle = cycle;
            }
        }

        cout << "\nСамый короткий Гамильтонов цикл (с минимальным весом " << minWeight << "): ";
        for (size_t k = 0; k < shortestCycle.size(); ++k) {
            cout << shortestCycle[k] + 1 << " ";
        }
        cout << shortestCycle[0] + 1 << endl;
    }
}


int** initGraph(int size) {
    int** G = createG(size);
    printG(G, size);
    return G;
}

int handleUserInputStart(int size) {
    int start;
    cout << "Введите номер стартовой вершины (1 - " << size << "): ";
    cin >> start;
    start -= 1;

    if (start < 0 || start >= size) {
        cout << "Неверный номер вершины!" << endl;
        exit(-1);
    }
    return start;
}

int handleUserInputSize() {
    int nG;
    cout << "Введите размер графа(от 1 - 20): ";
    cin >> nG;

    if (nG < 1 || nG>20) {
        cout << "Неверный размер графа!" << endl;
        exit(-1);
    }
    return nG;
}