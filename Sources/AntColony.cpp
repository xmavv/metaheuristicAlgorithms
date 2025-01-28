#include "../Headers/AntColony.h"
#include "../Headers/Utilities.h"
#include <algorithm>
#include <limits>
#include <iostream>
#include <chrono>
#include <cmath>
#include <iostream>
#include <cstdlib>
using namespace std;

AntColony::~AntColony() {
    for (int i = 0; i < numCities; ++i) {
        delete[] pheromones[i];
    }
    delete[] pheromones;
}

// Inicjalizacja populacji mrówek
void AntColony::initializePopulation(int**& population) {
    // Alokacja pamięci dla populacji
    population = new int*[populationSize];
    for (int i = 0; i < populationSize; ++i) {
        population[i] = new int[numCities]; // Alokacja pamięci dla trasy każdej mrówki
        for (int j = 0; j < numCities; ++j) {
            population[i][j] = j; // Inicjalizacja kolejności odwiedzania miast
        }
        // Losowe permutowanie miast, aby uzyskać różnorodne trasy początkowe
        std::shuffle(population[i], population[i] + numCities, rng);
    }
}

// Obliczanie długości trasy
double AntColony::calculateTourLength(const int* tour) {
    double length = 0.0;
    // Suma odległości pomiędzy kolejnymi miastami na trasie
    for (int i = 0; i < numCities - 1; ++i) {
        length += distanceMatrix[tour[i]][tour[i + 1]];
    }
    // Dodanie odległości powrotnej do miasta początkowego
    length += distanceMatrix[tour[numCities - 1]][tour[0]];
    return length;
}

// Aktualizacja macierzy feromonów
void AntColony::updatePheromones(int** population, const double* fitness, const int* bestTour, double bestTourLength) {
    // Odparowanie feromonów (redukcja wartości w macierzy)
    for (int i = 0; i < numCities; ++i) {
        for (int j = 0; j < numCities; ++j) {
            pheromones[i][j] *= (1.0 - evaporationRate); // Zmniejszanie feromonów o współczynnik parowania
            pheromones[i][j] = std::max(pheromones[i][j], minPheromone); // Dolne ograniczenie wartości feromonów
            pheromones[i][j] = std::min(pheromones[i][j], maxPheromone); // Górne ograniczenie wartości feromonów
        }
    }

    // Wzmocnienie feromonów na podstawie najlepszej trasy
    for (int i = 0; i < numCities - 1; ++i) {
        int from = bestTour[i];
        int to = bestTour[i + 1];
        pheromones[from][to] += Q / bestTourLength; // Aktualizacja na krawędziach trasy
        pheromones[to][from] += Q / bestTourLength; // Symetryczna aktualizacja
    }
}

// Lokalna optymalizacja trasy metodą 2-opt
void AntColony::twoOptLocalSearch(int* tour) {
    bool improvement = true; // Flaga wskazująca, czy znaleziono poprawę
    auto startTime = std::chrono::steady_clock::now(); // Czas rozpoczęcia optymalizacji
    while (improvement) {
        improvement = false;
        for (int i = 0; i < numCities - 1; ++i) {
            for (int j = i + 2; j < numCities; ++j) {
                if (j - i == 1) continue; // Pomijanie sąsiednich miast
                // Obliczanie zmiany długości trasy po zamianie krawędzi
                double delta =
                        distanceMatrix[tour[i]][tour[j]] + distanceMatrix[tour[i + 1]][tour[(j + 1) % numCities]] -
                        distanceMatrix[tour[i]][tour[i + 1]] - distanceMatrix[tour[j]][tour[(j + 1) % numCities]];
                if (delta < 0) {
                    // Jeśli zmiana jest korzystna, odwracamy podtrasę
                    std::reverse(tour + i + 1, tour + j + 1);
                    improvement = true;
                }
            }
        }
        // Przerwanie optymalizacji po przekroczeniu 100 ms
        auto currentTime = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count() > 100) {
            break;
        }
    }
}

// Główna metoda algorytmu z wykorzystaniem PMX
int* AntColony::runPMX(int numCities, double** distanceMatrix, int populationSize,
                       double alpha, double beta, double evaporationRate,
                       double crossoverRate, double mutationRate, int timeLimit) {
    // Inicjalizacja parametrów algorytmu
    this->numCities = numCities;
    this->distanceMatrix = distanceMatrix;
    this->populationSize = populationSize;
    this->alpha = alpha;
    this->beta = beta;
    this->evaporationRate = evaporationRate;
    this->crossoverRate = crossoverRate;
    this->mutationRate = mutationRate;
    this->timeLimit = timeLimit;

    double bladWzgledny;

    // Inicjalizacja macierzy feromonów
    pheromones = new double*[numCities];
    for (int i = 0; i < numCities; ++i) {
        pheromones[i] = new double[numCities];
        for (int j = 0; j < numCities; ++j) {
            pheromones[i][j] = initialPheromone; // Wartość początkowa feromonów
        }
    }

    // Tworzenie początkowej populacji
    int** population;
    initializePopulation(population);

    double* fitness = new double[populationSize]; // Tablica przechowująca dopasowanie tras
    int* bestTour = nullptr; // Najlepsza trasa
    double bestTourLength = std::numeric_limits<double>::max(); // Długość najlepszej trasy
    bladWzgledny = abs(bestTourLength - 2755)/2755;

    auto start = Utilities::startTimer(); // Rozpoczęcie pomiaru czasu
    double elapsedTimeMs;

    // Pętla główna algorytmu
    while (true) {
        elapsedTimeMs = Utilities::getElapsedTime(start);
        if (elapsedTimeMs > timeLimit) { // Sprawdzanie limitu czasu
            break;
        }

//        if((int)elapsedTimeMs % 10000 == 0) {
            cout << "blad wzgledny: " << bladWzgledny << endl;
//        }

        // Ocena populacji
        for (int i = 0; i < populationSize / 5; ++i) {
            twoOptLocalSearch(population[i]); // Optymalizacja lokalna trasy
            fitness[i] = 1.0 / calculateTourLength(population[i]); // Obliczanie dopasowania (odwrotność długości trasy)
            if (1.0 / fitness[i] < bestTourLength) {
                bestTourLength = 1.0 / fitness[i]; // Aktualizacja najlepszej długości trasy
                if (bestTour) delete[] bestTour; // Usunięcie starej najlepszej trasy
                bestTour = new int[numCities];
                std::copy(population[i], population[i] + numCities, bestTour); // Kopiowanie nowej najlepszej trasy
                bladWzgledny = abs(bestTourLength - 2755)/2755;
            }
        }

        // Aktualizacja feromonów
        updatePheromones(population, fitness, bestTour, bestTourLength);

        // Krzyżowanie i mutacje
        for (int i = 0; i < populationSize; ++i) {
            if (static_cast<double>(rand()) / RAND_MAX < crossoverRate) {
                int j = rand() % populationSize; // Wybór losowego rodzica
                int* child = crossoverPMX(population[i], population[j]); // Krzyżowanie PMX
                delete[] population[i];
                population[i] = child;
            }

            if (static_cast<double>(rand()) / RAND_MAX < mutationRate) {
                mutate(population[i]); // Mutacja trasy
            }
        }

        // Dynamiczna zmiana parametrów
        alpha = std::min(5.0, alpha + 0.001); // Zwiększanie znaczenia feromonów
        mutationRate = std::max(0.001, mutationRate * 0.99); // Zmniejszanie prawdopodobieństwa mutacji
    }

    // Usuwanie populacji
    for (int i = 0; i < populationSize; ++i) {
        delete[] population[i];
    }
    delete[] population;
    delete[] fitness;

    // Wyświetlenie wyników
    std::cout << "Najlepsza trasa: ";
    for (int i = 0; i < numCities; ++i) {
        std::cout << bestTour[i] << " ";
    }
    std::cout << "\nDlugosc najlepszej trasy: " << bestTourLength << std::endl;
    std::cout << "\nczas: " << elapsedTimeMs << std::endl;

    return bestTour; // Zwrócenie najlepszej trasy
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
    double bladWzgledny = abs(bestTourLength - 2755)/2755;

    auto start = Utilities::startTimer();

    int iteration = 0;
    double elapsedTimeMs;

    while (true) {
        elapsedTimeMs = Utilities::getElapsedTime(start);

        if (elapsedTimeMs > timeLimit) {
            break;
        }

        cout << "blad wzgledny: " << bladWzgledny << endl;

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

        // Aktualizacja feromonów
        updatePheromones(population, fitness, bestTour, bestTourLength);

        // Krzyżowanie i mutacje
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

        // Dynamiczna modyfikacja parametrów
        alpha = std::min(5.0, alpha + 0.001); // Zwiększanie znaczenia feromonów
        mutationRate = std::max(0.001, mutationRate * 0.99); // Zmniejszanie mutacji

        iteration++;
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

int* AntColony::crossoverOX(const int* parent1, const int* parent2) {
    int* child = new int[numCities];
    std::fill(child, child + numCities, -1);

    // Losowy wybór segmentu
    std::uniform_int_distribution<int> dist(0, numCities - 1);
    int start = dist(rng);
    int end = dist(rng);
    if (start > end) std::swap(start, end);

    // Kopiowanie segmentu z pierwszego rodzica
    for (int i = start; i <= end; ++i) {
        child[i] = parent1[i];
    }

    // Wypełnienie pozostałych pozycji zgodnie z kolejnością w drugim rodzicu
    int currentIdx = (end + 1) % numCities;
    for (int i = 0; i < numCities; ++i) {
        int gene = parent2[(end + 1 + i) % numCities];
        if (std::find(child + start, child + end + 1, gene) == child + end + 1) {
            child[currentIdx] = gene;
            currentIdx = (currentIdx + 1) % numCities;
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