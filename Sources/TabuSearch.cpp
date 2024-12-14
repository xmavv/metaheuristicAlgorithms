#include "../Headers/TabuSearch.h"
#include "../Headers/Utilities.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

// Funkcja obliczająca koszt trasy (TSP)
double TabuSearch::calculateCost(int* tour, int numCities, double** distanceMatrix) {
    double cost = 0.0;
    for (int i = 0; i < numCities - 1; ++i) {
        cost += distanceMatrix[tour[i]][tour[i + 1]];
    }
    cost += distanceMatrix[tour[numCities - 1]][tour[0]]; // Powrót do miasta początkowego
    return cost;
}

// Funkcja generująca losowe sąsiednie rozwiązanie
void TabuSearch::generateNeighbor(int* tour, int numCities) {
    int i = rand() % numCities;
    int j = rand() % numCities;
    while (i == j) { // Upewnij się, że wybrano różne miasta
        j = rand() % numCities;
    }
    swap(tour[i], tour[j]);
}

// Funkcja sprawdzająca, czy rozwiązanie jest na liście tabu
bool TabuSearch::isTabu(int* candidate, int numCities, int** tabuList, int tenure, int currentIteration) {
    for (int i = 0; i < numCities - 1; ++i) {
        if (tabuList[candidate[i]][candidate[i + 1]] > currentIteration) {
            return true;
        }
    }
    return false;
}

// Aktualizacja listy tabu
void TabuSearch::updateTabuList(int* solution, int numCities, int** tabuList, int tenure, int currentIteration) {
    for (int i = 0; i < numCities - 1; ++i) {
        tabuList[solution[i]][solution[i + 1]] = currentIteration + tenure;
    }
}

// Algorytm Tabu Search
