#include <iostream>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <chrono>
#include "../Headers/SimulatedAnnealing.h"

using namespace std;
using namespace chrono;

// Funkcja obliczająca koszt trasy (np. dla problemu komiwojażera)
double SimulatedAnnealing::calculateCost(int *tour, int numCities, double **distanceMatrix) {
    double cost = 0.0;
    for (int i = 0; i < numCities - 1; ++i) {
        cost += distanceMatrix[tour[i]][tour[i + 1]];
    }
    cost += distanceMatrix[tour[numCities - 1]][tour[0]]; // Koszt powrotu do miasta początkowego
    return cost;
}

// Funkcja generująca sąsiednie rozwiązanie: zamiana dwóch losowych miast w trasie
void SimulatedAnnealing::generateNeighbor(int* tour, int numCities) {
    int i = rand() % numCities;
    int j = rand() % numCities;
    while (i == j) { // Upewnij się, że wybrano różne miasta
        j = rand() % numCities;
    }
    swap(tour[i], tour[j]);
}

double SimulatedAnnealing::calculateInitialTemperature(int* tour, int numCities, double** distanceMatrix) {
    double maxChange = 0.0;
    double log99 = log(0.99);
    for (int i = 0; i < numCities; ++i) {
        for (int j = i + 1; j < numCities; ++j) {
            //obliczanie funkcji celu
            double newCost = calculateCost(tour, numCities, distanceMatrix);
            swap(tour[i], tour[j]);
            double oldCost = calculateCost(tour, numCities, distanceMatrix);
            maxChange = max(maxChange, abs(newCost - oldCost));
            swap(tour[i], tour[j]); // Przywrócenie trasy
        }
    }
    double expression = -(maxChange/10) / log99;

    return expression;
}

double SimulatedAnnealing::simulatedAnnealing(int* bestTour, int numCities, double** distanceMatrix,
                          double coolingRate, int maxIterations, int timeLimitMs) {
    // Inicjalizacja
    double bestCostFound;
    int* currentTour = new int[numCities];
    for (int i = 0; i < numCities; ++i) {
        currentTour[i] = i;
        bestTour[i] = i;

    }
    random_shuffle(currentTour, currentTour + numCities);
    int firstCity = currentTour[0];

    double currentCost = calculateCost(currentTour, numCities, distanceMatrix);
    double bestCost = currentCost;

    double initialTemperature = calculateInitialTemperature(currentTour, numCities, distanceMatrix);
    double temperature = initialTemperature;

    auto start = Utilities::startTimer();
    // Iteracje algorytmu
    for (int iteration = 0; iteration < maxIterations; ++iteration) {
        auto now = high_resolution_clock::now();
        double elapsedTimeMs = Utilities::getElapsedTime(start);
        if (elapsedTimeMs > timeLimitMs) {
            cout << "przekroczono limit czasu: " << elapsedTimeMs << " ms" << endl;
            break;
        }

        // Tworzenie sąsiedniego rozwiązania
        int* neighborTour = new int[numCities];
        copy(currentTour, currentTour + numCities, neighborTour);
        generateNeighbor(neighborTour, numCities);
        double neighborCost = calculateCost(neighborTour, numCities, distanceMatrix);

        // Kryterium akceptacji
        if (neighborCost < currentCost ||
            exp((currentCost - neighborCost) / temperature) > (rand() / double(neighborCost/2))) {
            copy(neighborTour, neighborTour + numCities, currentTour);
            currentCost = neighborCost;

            // Aktualizacja najlepszego rozwiązania
            if (currentCost < bestCost) {
                bestCost = currentCost;
                copy(currentTour, currentTour + numCities, bestTour);
                bestCostFound = Utilities::getElapsedTime(start);
            }
        }

        delete[] neighborTour;

        // Schładzanie temperatury
        temperature *= coolingRate;

        // Przerwanie, jeśli temperatura jest bardzo niska
        if (temperature < 1e-6) {
            cout<<"temperatura za niska";
            break;
        }
    }

//    bestCost += distanceMatrix[firstCity][0];
//    bestTour[numCities] = firstCity;
//    cout << "ostatni element w path: " << bestTour[numCities] << endl;

    double elapsedTime = Utilities::getElapsedTime(start);
    cout << "czas algorytmu: " << elapsedTime << " ms" << endl<<endl;
    cout << "czas w ktorym znaleziono najlepsze rozwiazanie: " << bestCostFound << " ms" << endl;

    cout << "poczatkowa temperatura: " << initialTemperature << endl;
//    cout << "koncowa temperatura: " << exp(-1/temperature) << endl;
    cout << "koncowa temperatura: " << temperature << endl;

    delete[] currentTour;
    return bestCost;
}

int* SimulatedAnnealing::algorithm(double **incidenceMatrix, int len, double coolingRate, int maxIterations,
                                   int timeLimitMs){
    int* bestTour = new int[len];

    double bestCost = simulatedAnnealing(bestTour, len, incidenceMatrix, coolingRate, maxIterations, timeLimitMs);

    // Wyświetlenie najlepszego rozwiązania
    cout << "najlepsza trasa: ";
    for (int i = 0; i < len; ++i) {
        cout << bestTour[i] << " ";
    }
    cout << endl;
    cout << "minimalny koszt: " << bestCost << endl;

    return bestTour;
}