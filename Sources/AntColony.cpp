#include "../Headers/AntColony.h"
#include <algorithm>
#include <limits>
#include <chrono>

//AntColony::AntColony(int numCities, double** distanceMatrix, int populationSize,
//                     double alpha, double beta, double evaporationRate,
//                     double crossoverRate, double mutationRate, int timeLimit)
//        : numCities(numCities), distanceMatrix(distanceMatrix), populationSize(populationSize),
//          alpha(alpha), beta(beta), evaporationRate(evaporationRate),
//          crossoverRate(crossoverRate), mutationRate(mutationRate), timeLimit(timeLimit),
//          rng(static_cast<unsigned>(std::time(nullptr))) {
//
//}

AntColony::~AntColony() {
    for (int i = 0; i < numCities; ++i) {
        delete[] pheromones[i];
    }
    delete[] pheromones;
}

void AntColony::initializePopulation(int**& population) {
    population = new int*[populationSize];
    for (int i = 0; i < populationSize; ++i) {
        population[i] = new int[numCities];
        for (int j = 0; j < numCities; ++j) {
            population[i][j] = j;
        }
        std::shuffle(population[i], population[i] + numCities, rng);
    }
}

double AntColony::calculateTourLength(const int* tour) {
    double length = 0.0;
    for (int i = 0; i < numCities - 1; ++i) {
        length += distanceMatrix[tour[i]][tour[i + 1]];
    }
    length += distanceMatrix[tour[numCities - 1]][tour[0]];
    return length;
}

void AntColony::updatePheromones(int** population, const double* fitness) {
    for (int i = 0; i < numCities; ++i) {
        for (int j = 0; j < numCities; ++j) {
            pheromones[i][j] *= (1.0 - evaporationRate);
        }
    }

    for (int k = 0; k < populationSize; ++k) {
        for (int i = 0; i < numCities - 1; ++i) {
            int from = population[k][i];
            int to = population[k][i + 1];
            pheromones[from][to] += fitness[k];
            pheromones[to][from] += fitness[k];
        }
    }
}

int* AntColony::crossoverOX(const int* parent1, const int* parent2) {
    int* child = new int[numCities];
    std::fill(child, child + numCities, -1);

    std::uniform_int_distribution<int> dist(0, numCities - 1);
    int start = dist(rng);
    int end = dist(rng);
    if (start > end) std::swap(start, end);

    for (int i = start; i <= end; ++i) {
        child[i] = parent1[i];
    }

    int currentIdx = (end + 1) % numCities;
    for (int i = 0; i < numCities; ++i) {
        int gene = parent2[(end + 1 + i) % numCities];
        if (std::find(child, child + numCities, gene) == child + numCities) {
            child[currentIdx] = gene;
            currentIdx = (currentIdx + 1) % numCities;
        }
    }

    return child;
}

int* AntColony::crossoverPMX(const int* parent1, const int* parent2) {
    int* child = new int[numCities];
    std::fill(child, child + numCities, -1);

    std::uniform_int_distribution<int> dist(0, numCities - 1);
    int start = dist(rng);
    int end = dist(rng);
    if (start > end) std::swap(start, end);

    for (int i = start; i <= end; ++i) {
        child[i] = parent1[i];
    }

    for (int i = start; i <= end; ++i) {
        int gene = parent2[i];
        if (std::find(child, child + numCities, gene) == child + numCities) {
            int pos = i;
            while (pos >= start && pos <= end) {
                gene = parent1[pos];
                pos = std::find(parent2, parent2 + numCities, gene) - parent2;
            }
            child[pos] = parent2[i];
        }
    }

    for (int i = 0; i < numCities; ++i) {
        if (child[i] == -1) {
            child[i] = parent2[i];
        }
    }

    return child;
}

void AntColony::mutate(int* chromosome) {
    std::uniform_int_distribution<int> dist(0, numCities - 1);
    int pos1 = dist(rng);
    int pos2 = dist(rng);
    std::swap(chromosome[pos1], chromosome[pos2]);
}

int* AntColony::run(int numCities, double** distanceMatrix, int populationSize,
                    double alpha, double beta, double evaporationRate,
                    double crossoverRate, double mutationRate, int timeLimit) {
    this->numCities = numCities;
    this->distanceMatrix = distanceMatrix;
    this->populationSize = populationSize;
    this->alpha = alpha;
    this->beta = beta;
    this->evaporationRate = evaporationRate;
    this->crossoverRate = crossoverRate;
    this->mutationRate = mutationRate;
    this->timeLimit = timeLimit;

    pheromones = new double*[numCities];
    for (int i = 0; i < numCities; ++i) {
        pheromones[i] = new double[numCities];
        for (int j = 0; j < numCities; ++j) {
            pheromones[i][j] = 1.0;
        }
    }

    int** population;
    initializePopulation(population);

    double* fitness = new double[populationSize];
    int* bestTour = nullptr;
    double bestTourLength = std::numeric_limits<double>::max();

    auto startTime = std::chrono::steady_clock::now();

    while (true) {
        auto currentTime = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count() > timeLimit) {
            break;
        }

        for (int i = 0; i < populationSize; ++i) {
            fitness[i] = 1.0 / calculateTourLength(population[i]);
        }

        for (int i = 0; i < populationSize; ++i) {
            if (1.0 / fitness[i] < bestTourLength) {
                bestTourLength = 1.0 / fitness[i];
                if (bestTour) delete[] bestTour;
                bestTour = new int[numCities];
                std::copy(population[i], population[i] + numCities, bestTour);
            }
        }

        updatePheromones(population, fitness);

        for (int i = 0; i < populationSize; ++i) {
            if (static_cast<double>(rand()) / RAND_MAX < crossoverRate) {
                int j = rand() % populationSize;
                int* child = crossoverOX(population[i], population[j]);
                delete[] population[i];
                population[i] = child;
            }

            if (static_cast<double>(rand()) / RAND_MAX < mutationRate) {
                mutate(population[i]);
            }
        }
    }

    for (int i = 0; i < populationSize; ++i) {
        delete[] population[i];
    }
    delete[] population;
    delete[] fitness;

    std::cout << "Najlepsza trasa: ";
    for (int i = 0; i < numCities; ++i) {
        std::cout << bestTour[i] << " ";
    }
    std::cout << "\nDlugosc najlepszej trasy: " << bestTourLength << std::endl;

    return bestTour;
}
