#ifndef UNTITLED1_SIMULATEDANNEALING_H
#define UNTITLED1_SIMULATEDANNEALING_H

#include "Utilities.h"
using namespace std;

class SimulatedAnnealing {
public:
    double calculateCost(int* tour, int numCities, double** distanceMatrix);
    void generateNeighbor(int* tour, int numCities);
    double calculateInitialTemperature(int* tour, int numCities, double** distanceMatrix);
    double simulatedAnnealing(int* bestTour, int numCities, double** distanceMatrix, double coolingRate, int maxIterations, int timeLimitMs);
    int* algorithm(double **incidenceMatrix, int len, double coolingRate, int maxIterations, int timeLimitMs);
};

#endif //UNTITLED1_SIMULATEDANNEALING_H
