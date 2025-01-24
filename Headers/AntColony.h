#include <iostream>
#include <algorithm>
#include <random>
#include <ctime>

using namespace std;
#include <random>

// AntColony.h
#ifndef ANT_COLONY_H
#define ANT_COLONY_H

#include <random>

class AntColony {
private:
    int numCities;
    double** distanceMatrix;
    int populationSize;
    double alpha, beta, evaporationRate;
    double crossoverRate, mutationRate;
    int timeLimit;
    double** pheromones;
    std::mt19937 rng;

    void initializePopulation(int**& population);
    double calculateTourLength(const int* tour);
    void updatePheromones(int** population, const double* fitness);

    int* crossoverOX(const int* parent1, const int* parent2);
    int* crossoverPMX(const int* parent1, const int* parent2);
    void mutate(int* chromosome);

public:
    ~AntColony();

    int* run(int numCities, double** distanceMatrix, int populationSize,
             double alpha, double beta, double evaporationRate,
             double crossoverRate, double mutationRate, int timeLimit);
};

#endif // ANT_COLONY_H
