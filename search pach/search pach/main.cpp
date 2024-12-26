#include "GraphOperations.h"
#include "Rendering.h"
#include <SFML/Graphics.hpp>
#include <locale>
#include <ctime>
#include <iostream>

using namespace std;

int main() {
    srand(time(NULL));
    setlocale(LC_ALL, "Russian");


    int flag = 0;
    int nG = 0;
    int** G = 0;

    cout << "Рандомная генерация/ручной ввод(1/2): ";
    cin >> flag;

    if (flag < 1 || flag > 2) {
        cout << "Неверный выбор!" << endl;
        exit(-1);
    }

    if (flag == 1) {
        nG = handleUserInputSize();
        G = initGraph(nG);
    }
    else {
        nG = handleUserInputSize();
        G = new int* [nG];
        cout << "Введите граф размером " << nG << ", числа от 0 до 7: " << endl;
        for (int i = 0; i < nG; i++) {
            G[i] = new int[nG];
            for (int j = 0; j < nG; j++) {
                cout << "Элемент графа[" << i + 1 << "][" << j + 1 << "]: ";
                cin >> G[i][j];
                if (G[i][j] < 0 || G[i][j]>7) {
                    cout << "Неверный выбор!" << endl;
                    exit(-1);
                }
            }
        }
        printG(G, nG);
    }

    int start = handleUserInputStart(nG);
    findAllHamiltonianCycles(G, nG, start);
    runGraphicsWindow(G, nG);

    return 0;
}