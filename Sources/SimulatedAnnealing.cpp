#include <iostream>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <chrono>
#include "../Headers/SimulatedAnnealing.h"

using namespace std;
using namespace chrono;

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
            swap(tour[i], tour[j]);
        }
    }
    double expression = -(maxChange/10) / log99;

    return expression;
}

double SimulatedAnnealing::simulatedAnnealing(int* bestTour, int numCities, double** distanceMatrix,
                          double coolingRate, int maxIterations, int timeLimitMs) {
    double bestCostFound;
    double bladWzgledny = 100;

    int* currentTour = new int[numCities];
    for (int i = 0; i < numCities; ++i) {
        currentTour[i] = i;
        bestTour[i] = i;

    }
    random_shuffle(currentTour, currentTour + numCities);

    double currentCost = calculateCost(currentTour, numCities, distanceMatrix);
    double bestCost = currentCost;

    double initialTemperature = calculateInitialTemperature(currentTour, numCities, distanceMatrix);
    double temperature = initialTemperature;

    auto start = Utilities::startTimer();

    for (int iteration = 0; iteration < maxIterations; ++iteration) {
        auto now = high_resolution_clock::now();
        double elapsedTimeMs = Utilities::getElapsedTime(start);

        bladWzgledny = (abs(bestCost - 1608)/1608) * 100;

        cout << bladWzgledny << ", ";
        if (elapsedTimeMs > timeLimitMs) {
            cout << "przekroczono limit czasu: " << elapsedTimeMs << " ms" << endl;
            break;
        }

        int* neighborTour = new int[numCities];
        copy(currentTour, currentTour + numCities, neighborTour);
        generateNeighbor(neighborTour, numCities);
        double neighborCost = calculateCost(neighborTour, numCities, distanceMatrix);

        // gdy nasz koszt jest lepszy to go akceptujemy, lub gdy nie jest lepszy ale temperatura nam na to pozwala
        if (neighborCost < currentCost ||
            exp((currentCost - neighborCost) / temperature) > (rand() / double(neighborCost))) {
            copy(neighborTour, neighborTour + numCities, currentTour);
            currentCost = neighborCost;

            // ustawienie najlepszego rozwiazania i zapisanie czasu
            if (currentCost < bestCost) {
                bestCost = currentCost;
                copy(currentTour, currentTour + numCities, bestTour);
                bestCostFound = Utilities::getElapsedTime(start);
            }
        }

        delete[] neighborTour;

        temperature *= coolingRate;

        // przerywamy gdy temperatura jest za niska
        if (temperature < 1e-6) {
            cout<<"temperatura za niska";
            break;
        }
    }

    double elapsedTime = Utilities::getElapsedTime(start);
    cout << "czas algorytmu: " << elapsedTime << " ms" << endl<<endl;

    cout << "poczatkowa temperatura: " << initialTemperature << endl;
    cout << "koncowa temperatura: " << temperature << endl;

    delete[] currentTour;
    return bestCost;
}

int* SimulatedAnnealing::algorithm(double **incidenceMatrix, int len, double coolingRate, int maxIterations,
                                   int timeLimitMs){
    int* bestTour = new int[len];

    double bestCost = simulatedAnnealing(bestTour, len, incidenceMatrix, coolingRate, maxIterations, timeLimitMs);

    cout << "najlepsza trasa: ";
    for (int i = 0; i < len; ++i) {
        cout << bestTour[i] << " ";
    }
    cout << endl;
    cout << "minimalny koszt: " << bestCost << endl;

    return bestTour;
}