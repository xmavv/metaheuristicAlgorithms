#ifndef UNTITLED1_GREEDY_H
#define UNTITLED1_GREEDY_H
#include <vector>
#include <limits>
#include <iomanip>
#include <iostream>

#endif //UNTITLED1_GREEDY_H

using namespace std;

struct GreedyResult {
    vector<int> path; // Znaleziona trasa
    double cost;      // Całkowity koszt trasy
};

class Greedy {
public:
    GreedyResult greedyAlgorithm(double** distanceMatrix, int numVertices);
};