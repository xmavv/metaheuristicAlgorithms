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
int* TabuSearch::tabuSearch(double** distanceMatrix, int numCities, int maxIterations, int tabuTenure, int timeLimitMs) {
    // Inicjalizacja zmiennych
    int* currentSolution = new int[numCities];
    int* bestSolution = new int[numCities];
    for (int i = 0; i < numCities; ++i) {
        currentSolution[i] = i;
        bestSolution[i] = i;
    }
    random_shuffle(currentSolution, currentSolution + numCities);

    double currentCost = calculateCost(currentSolution, numCities, distanceMatrix);
    double bestCost = currentCost;

    int** tabuList = new int*[numCities];
    for (int i = 0; i < numCities; ++i) {
        tabuList[i] = new int[numCities];
        fill(tabuList[i], tabuList[i] + numCities, 0);
    }

    auto start = Utilities::startTimer();

    for (int iteration = 0; iteration < maxIterations; ++iteration) {
        double elapsedTimeMs = Utilities::getElapsedTime(start);
        if (elapsedTimeMs > timeLimitMs) {
            cout << "przekroczono limit czasu: " << elapsedTimeMs << " ms" << endl;
            break;
        }

        int* bestNeighbor = new int[numCities];
        copy(currentSolution, currentSolution + numCities, bestNeighbor);
        double bestNeighborCost = INT_MAX;

        // Szukanie najlepszego sąsiada
        for (int i = 0; i < numCities; ++i) { // Generuj więcej sąsiadów
            int* neighbor = new int[numCities];
            copy(currentSolution, currentSolution + numCities, neighbor);

            // Wybierz losową strategię generowania sąsiada
            generateNeighbor(neighbor, numCities); // Zamiana dwóch miast

            // Sprawdzenie tabu i aktualizacja najlepszego sąsiada
            double neighborCost = calculateCost(neighbor, numCities, distanceMatrix);
            if (neighborCost < bestNeighborCost) {
                bestNeighborCost = neighborCost;
                copy(neighbor, neighbor + numCities, bestNeighbor);
            }

            delete[] neighbor;
        }

        // Aktualizacja najlepszego rozwiązania
        if (!isTabu(bestNeighbor, numCities, tabuList, tabuTenure, iteration) || bestNeighborCost < bestCost) {
            bestCost = bestNeighborCost;
            copy(bestNeighbor, bestNeighbor + numCities, bestSolution);
        }

        // Aktualizacja obecnego rozwiązania i listy tabu
        copy(bestNeighbor, bestNeighbor + numCities, currentSolution);
        updateTabuList(currentSolution, numCities, tabuList, tabuTenure, iteration);

        delete[] bestNeighbor;
    }

    double elapsedTime = Utilities::getElapsedTime(start);
    cout << "Czas algorytmu: " << elapsedTime << " ms" << endl;
    cout << "Najlepsza trasa: ";
    for (int i = 0; i < numCities; ++i) {
        cout << bestSolution[i] << " ";
    }
    cout << endl;
    cout << "Minimalny koszt: " << bestCost << endl;

    // Zwolnienie pamięci dla listy tabu
    for (int i = 0; i < numCities; ++i) {
        delete[] tabuList[i];
    }
    delete[] tabuList;
    delete[] currentSolution;

    return bestSolution; // Zwracamy najlepsze rozwiązanie
}