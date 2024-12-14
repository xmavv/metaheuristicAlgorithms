#include "../Headers/Algorithm.h"
#include <algorithm>
using namespace std;

double Algorithm::calculateCost(int* tour, int numCities, double** distanceMatrix) {
    double cost = 0.0;
    for (int i = 0; i < numCities - 1; ++i) {
        cost += distanceMatrix[tour[i]][tour[i + 1]];
    }
    cost += distanceMatrix[tour[numCities - 1]][tour[0]]; // Powrót do miasta początkowego
    return cost;
}

void Algorithm::generateNeighbor(int *tour, int numCities) {
    int i = rand() % numCities;
    int j = rand() % numCities;
    while (i == j) { // Upewnij się, że wybrano różne miasta
        j = rand() % numCities;
    }
    swap(tour[i], tour[j]);
}