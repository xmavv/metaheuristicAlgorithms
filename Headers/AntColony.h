#ifndef ANT_COLONY_H
#define ANT_COLONY_H

#include <iostream>
#include <algorithm>
#include <random>
#include <ctime>
#include <vector>

class AntColony {
private:
    // Parametry algorytmu
    int numCities;
    double** distanceMatrix;
    int populationSize;
    double alpha, beta, evaporationRate;
    double crossoverRate, mutationRate;
    int timeLimit;

    // Pheromone matrix
    double** pheromones;

    // Generator losowy
    std::mt19937 rng;

    // Parametry dodatkowe
    double initialPheromone = 1.0;
    double minPheromone = 0.01;
    double maxPheromone = 10.0;
    double Q = 100.0; // Stała do aktualizacji feromonów

    // Metody pomocnicze
    void initializePopulation(int**& population);
    double calculateTourLength(const int* tour);
    void updatePheromones(int** population, const double* fitness, const int* bestTour, double bestTourLength);
    void twoOptLocalSearch(int* tour);

    // Metody genetyczne
    int* crossoverOX(const int* parent1, const int* parent2);
    int* crossoverPMX(const int* parent1, const int* parent2);
    void mutate(int* chromosome);

public:
    // Destruktor
    ~AntColony();

    // Główna funkcja algorytmu
    int* runOX(int numCities, double** distanceMatrix, int populationSize,
             double alpha, double beta, double evaporationRate,
             double crossoverRate, double mutationRate, int timeLimit);

    int* runPMX(int numCities, double** distanceMatrix, int populationSize,
               double alpha, double beta, double evaporationRate,
               double crossoverRate, double mutationRate, int timeLimit);
};

#endif // ANT_COLONY_H
