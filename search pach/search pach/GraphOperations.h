#ifndef GRAPH_OPERATIONS_H
#define GRAPH_OPERATIONS_H

#include <vector>

int** createG(int size);
void printG(int** G, int size);
bool isSafe(int v, int** G, std::vector<int>& path, int pos);
void hamiltonianCycleUtil(int** G, std::vector<int>& path, int pos, int size, std::vector<std::vector<int>>& allPaths);
int calculateCycleWeight(int** G, const std::vector<int>& path);
void findAllHamiltonianCycles(int** G, int size, int start);
int** initGraph(int size);
int handleUserInput(int size);

#endif
