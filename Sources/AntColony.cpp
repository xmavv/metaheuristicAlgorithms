#include "../Headers/AntColony.h"
#include "../Headers/Utilities.h"
#include <algorithm>
#include <limits>
#include <chrono>
#include <cmath>

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

void AntColony::updatePheromones(int** population, const double* fitness, const int* bestTour, double bestTourLength) {
    // Odparowanie feromonów
    for (int i = 0; i < numCities; ++i) {
        for (int j = 0; j < numCities; ++j) {
            pheromones[i][j] *= (1.0 - evaporationRate);
            pheromones[i][j] = std::max(pheromones[i][j], minPheromone);
            pheromones[i][j] = std::min(pheromones[i][j], maxPheromone);
        }
    }

    // Aktualizacja feromonów tylko na podstawie najlepszej trasy
    for (int i = 0; i < numCities - 1; ++i) {
        int from = bestTour[i];
        int to = bestTour[i + 1];
        pheromones[from][to] += Q / bestTourLength;
        pheromones[to][from] += Q / bestTourLength;
    }
}

void AntColony::twoOptLocalSearch(int* tour) {
    bool improvement = true;
    auto startTime = std::chrono::steady_clock::now();
    while (improvement) {
        improvement = false;
        for (int i = 0; i < numCities - 1; ++i) {
            for (int j = i + 2; j < numCities; ++j) {
                if (j - i == 1) continue; // Sąsiadujące miasta
                double delta =
                        distanceMatrix[tour[i]][tour[j]] + distanceMatrix[tour[i + 1]][tour[(j + 1) % numCities]] -
                        distanceMatrix[tour[i]][tour[i + 1]] - distanceMatrix[tour[j]][tour[(j + 1) % numCities]];
                if (delta < 0) {
                    std::reverse(tour + i + 1, tour + j + 1);
                    improvement = true;
                }
            }
        }
        // Ograniczenie czasu działania 2-opt
        auto currentTime = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count() > 100) {
            break;
        }
    }
}

int* AntColony::runPMX(int numCities, double** distanceMatrix, int populationSize,
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
            pheromones[i][j] = initialPheromone;
        }
    }

    int** population;
    initializePopulation(population);

    double* fitness = new double[populationSize];
    int* bestTour = nullptr;
    double bestTourLength = std::numeric_limits<double>::max();

    auto start = Utilities::startTimer();

    int iteration = 0;
    double elapsedTimeMs;

    while (true) {
        cout<< "zaczynam" <<endl;
        elapsedTimeMs = Utilities::getElapsedTime(start);

        if (elapsedTimeMs > timeLimit) {
            break;
        }

        cout<< "ocena" <<endl;

        // Ocena populacji
        for (int i = 0; i < populationSize/5; ++i) {
            twoOptLocalSearch(population[i]); // Lokalne ulepszenie
            fitness[i] = 1.0 / calculateTourLength(population[i]);
            if (1.0 / fitness[i] < bestTourLength) {
                bestTourLength = 1.0 / fitness[i];
                if (bestTour) delete[] bestTour;
                bestTour = new int[numCities];
                std::copy(population[i], population[i] + numCities, bestTour);
            }
        }

        cout<< "feromony" <<endl;

        // Aktualizacja feromonów
        updatePheromones(population, fitness, bestTour, bestTourLength);

        cout<< "krzyzowanie i mutacje" <<endl;

        // Krzyżowanie i mutacje
        for (int i = 0; i < populationSize; ++i) {
            if (static_cast<double>(rand()) / RAND_MAX < crossoverRate) {
                int j = rand() % populationSize;
                int* child = crossoverPMX(population[i], population[j]);
                delete[] population[i];
                population[i] = child;
            }

            if (static_cast<double>(rand()) / RAND_MAX < mutationRate) {
                mutate(population[i]);
            }
        }

        cout<< "dynamiczna modyfikacja parametrow xd" <<endl;

        // Dynamiczna modyfikacja parametrów
        alpha = std::min(5.0, alpha + 0.001); // Zwiększanie znaczenia feromonów
        mutationRate = std::max(0.001, mutationRate * 0.99); // Zmniejszanie mutacji

        iteration++;
        cout<< "koncze" <<endl;
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
    std::cout << "\nczas: " << elapsedTimeMs << std::endl;

    return bestTour;
}

int* AntColony::runOX(int numCities, double** distanceMatrix, int populationSize,
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
            pheromones[i][j] = initialPheromone;
        }
    }

    int** population;
    initializePopulation(population);

    double* fitness = new double[populationSize];
    int* bestTour = nullptr;
    double bestTourLength = std::numeric_limits<double>::max();

    auto start = Utilities::startTimer();

    int iteration = 0;
    double elapsedTimeMs;

    while (true) {
        cout<< "zaczynam" <<endl;
        elapsedTimeMs = Utilities::getElapsedTime(start);

        if (elapsedTimeMs > timeLimit) {
            break;
        }

        cout<< "ocena" <<endl;

        // Ocena populacji
        for (int i = 0; i < populationSize/5; ++i) {
            twoOptLocalSearch(population[i]); // Lokalne ulepszenie
            fitness[i] = 1.0 / calculateTourLength(population[i]);
            if (1.0 / fitness[i] < bestTourLength) {
                bestTourLength = 1.0 / fitness[i];
                if (bestTour) delete[] bestTour;
                bestTour = new int[numCities];
                std::copy(population[i], population[i] + numCities, bestTour);
            }
        }

        cout<< "feromony" <<endl;

        // Aktualizacja feromonów
        updatePheromones(population, fitness, bestTour, bestTourLength);

        cout<< "krzyzowanie i mutacje" <<endl;

        // Krzyżowanie i mutacje
        for (int i = 0; i < populationSize; ++i) {
            if (static_cast<double>(rand()) / RAND_MAX < crossoverRate) {
                int j = rand() % populationSize;
                int* child = crossoverPMX(population[i], population[j]);
                delete[] population[i];
                population[i] = child;
            }

            if (static_cast<double>(rand()) / RAND_MAX < mutationRate) {
                mutate(population[i]);
            }
        }

        cout<< "dynamiczna modyfikacja parametrow xd" <<endl;

        // Dynamiczna modyfikacja parametrów
        alpha = std::min(5.0, alpha + 0.001); // Zwiększanie znaczenia feromonów
        mutationRate = std::max(0.001, mutationRate * 0.99); // Zmniejszanie mutacji

        iteration++;
        cout<< "koncze" <<endl;
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
    std::cout << "\nczas: " << elapsedTimeMs << std::endl;

    return bestTour;
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
