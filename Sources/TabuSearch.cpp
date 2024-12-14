#include "../Headers/TabuSearch.h"
#include "../Headers/Utilities.h"
#include <iostream>
#include <algorithm>
#include <climits>
using namespace std;

bool TabuSearch::isTabu(int* candidate, int numCities, int** tabuList, int tenure, int currentIteration) {
    for (int i = 0; i < numCities - 1; ++i) {
        if (tabuList[candidate[i]][candidate[i + 1]] > currentIteration) {
            return true;
        }
    }
    return false;
}

void TabuSearch::updateTabuList(int* solution, int numCities, int** tabuList, int tenure, int currentIteration) {
    for (int i = 0; i < numCities - 1; ++i) {
        tabuList[solution[i]][solution[i + 1]] = currentIteration + tenure;
    }
}

int* TabuSearch::tabuSearch(double** distanceMatrix, int numCities, int maxIterations, int tabuTenure, int timeLimitMs) {
    int* currentSolution = new int[numCities];
    int* bestSolution = new int[numCities];
    double bestSolutionTime;

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

        // szukamy najlepszego mozliwego sasiada iterujac po wszystkich sasiadach
        for (int i = 0; i < numCities; ++i) {
            int* neighbor = new int[numCities];
            copy(currentSolution, currentSolution + numCities, neighbor);

            generateNeighbor(neighbor, numCities); // Zamiana dwóch miast

            double neighborCost = calculateCost(neighbor, numCities, distanceMatrix);
            //znalezenie najlepszego sasiada
            if (neighborCost < bestNeighborCost) {
                bestNeighborCost = neighborCost;
                copy(neighbor, neighbor + numCities, bestNeighbor);
            }

            delete[] neighbor;
        }

        // sprawdzeine listy tabu i dodanie najlepszego sasiada
        if (!isTabu(bestNeighbor, numCities, tabuList, tabuTenure, iteration) || bestNeighborCost < bestCost) {
            bestCost = bestNeighborCost;
            copy(bestNeighbor, bestNeighbor + numCities, bestSolution);
            bestSolutionTime = Utilities::getElapsedTime(start);
        }

        // updateujemy liste tabu i aktualne rozwiazanie ktore bedzie przegladane dalej
        copy(bestNeighbor, bestNeighbor + numCities, currentSolution);
        updateTabuList(currentSolution, numCities, tabuList, tabuTenure, iteration);

        delete[] bestNeighbor;
    }

    double elapsedTime = Utilities::getElapsedTime(start);
    cout << "czas algorytmu: " << elapsedTime << " ms" << endl;
    cout << "czas w ktorym znaleziono najlepsze rozwiazanie: " << bestSolutionTime << " ms" << endl;

    cout << "najlepsza trasa: ";
    for (int i = 0; i < numCities; ++i) {
        cout << bestSolution[i] << " ";
    }
    cout << endl;
    cout << "minimalny koszt: " << bestCost << endl;

    for (int i = 0; i < numCities; ++i) {
        delete[] tabuList[i];
    }
    delete[] tabuList;
    delete[] currentSolution;

    return bestSolution;
}