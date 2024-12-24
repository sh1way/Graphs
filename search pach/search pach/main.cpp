#include "GraphOperations.h"
#include "Rendering.h"
#include <SFML/Graphics.hpp>
#include <locale>
#include <ctime>

int main() {
    srand(time(NULL));
    setlocale(LC_ALL, "Russian");

    const int nG = 7;
    int** G = initGraph(nG);
    int start = handleUserInput(nG);
    findAllHamiltonianCycles(G, nG, start);
    runGraphicsWindow(G, nG);

    return 0;
}