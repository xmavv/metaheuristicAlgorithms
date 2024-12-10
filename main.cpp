#include <iostream>
#include <vector>
#include "Headers/Menu.h"
//#include "Sources/SimulatedAnnealing.cpp"
#include "Headers/SimulatedAnnealing.h"

int main() {
    Menu menu;
    menu.run();

//    srand(time(0));
//
//    // Rozmiar macierzy kosztów
//    int numCities = 5;
//    SimulatedAnnealing sa;
//
//    // Alokacja dynamiczna macierzy kosztów
//    double** distanceMatrix = new double*[numCities];
//    for (int i = 0; i < numCities; ++i) {
//        distanceMatrix[i] = new double[numCities];
//    }
//
//    // Przykładowa macierz kosztów
//    double exampleMatrix[5][5] = {
//            { -1,  10,  15,  20,  25 },
//            { 10,  -1,  35,  25,  30 },
//            { 15,  35,  -1,  30,  20 },
//            { 20,  25,  30,  -1,  15 },
//            { 25,  30,  20,  15,  -1 }
//    };
//
//    // Kopiowanie do dynamicznej macierzy
//    for (int i = 0; i < numCities; ++i) {
//        for (int j = 0; j < numCities; ++j) {
//            distanceMatrix[i][j] = exampleMatrix[i][j];
//        }
//    }
//
//    // Inicjalizacja zmiennych algorytmu
//    int* bestTour = new int[numCities];
//    double coolingRate = 0.95;
//    int maxIterations = 100000;
//    int timeLimitMs = 5000; // Limit czasu: 5 sekund
//
//    // Wywołanie algorytmu
//    double bestCost = sa.simulatedAnnealing(bestTour, numCities, distanceMatrix, coolingRate, maxIterations, timeLimitMs);
//
//    // Wyświetlenie najlepszego rozwiązania
//    cout << "Najlepsza trasa: ";
//    for (int i = 0; i < numCities; ++i) {
//        cout << bestTour[i] << " ";
//    }
//    cout << endl;
//    cout << "Minimalny koszt: " << bestCost << endl;
//
//    // Zwolnienie pamięci
//    delete[] bestTour;
//    for (int i = 0; i < numCities; ++i) {
//        delete[] distanceMatrix[i];
//    }
//    delete[] distanceMatrix;

    return 0;
}
