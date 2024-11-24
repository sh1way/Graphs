#include <SFML/Graphics.hpp>
#include <locale>
#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

int** createG(int size) {
    int** G = (int**)malloc(size * sizeof(int*));
    for (int i = 0; i < size; i++) {
        G[i] = (int*)malloc(size * sizeof(int));
    }

    for (int i = 0; i < size; i++) {
        for (int j = i; j < size; j++) {
            if (i == j) {
                G[i][j] = 0;
            }
            else {
                G[i][j] = rand() % 2;
                G[j][i] = G[i][j];
            }
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

int sumEdges(int** G, int size, int& edges) {
    int edge = 0;
    for (int i = 0; i < size; i++) {
        for (int j = 0;j < size; j++) {
            if (G[i][j] == 1) {
                edge++;
            }
        }
    }
    edges = edge / 2;
    return edges;
}

int main() {
    srand(time(NULL));
    setlocale(LC_ALL, "");

    int nG = 6;
    int edges = 0;
    int** G = createG(nG);

    printG(G, nG);
    sumEdges(G, nG, edges);

    cout << "\n" << edges << endl;

    return 0;
}