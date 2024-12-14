#include "../Headers/Greedy.h"


int* Greedy::greedyAlgorithm(double** distanceMatrix, int numVertices) {
    bool* visited = new bool[numVertices]; // Dynamiczna tablica oznaczająca odwiedzone miasta
    int* path = new int[numVertices + 1]; // Tablica trasy (zawiera powrót do miasta początkowego)
    double totalCost = 0.0;               // Całkowity koszt trasy

    for (int i = 0; i < numVertices; ++i) {
        visited[i] = false;
    }

    int currentCity = 0; // Zaczynamy od miasta 0
    visited[currentCity] = true;
    path[0] = currentCity;

    // Znajdowanie kolejnych miast
    for (int step = 1; step < numVertices; ++step) {
        double minCost = numeric_limits<double>::max();
        int nextCity = -1;

        // Szukanie najtańszego nieodwiedzonego miasta
        for (int city = 0; city < numVertices; ++city) {
            if (!visited[city] && distanceMatrix[currentCity][city] != -1) { // Nieodwiedzone i połączenie istnieje
                if (distanceMatrix[currentCity][city] < minCost) {
                    minCost = distanceMatrix[currentCity][city];
                        nextCity = city;
                }
            }
        }

        // Jeśli nie znaleziono sąsiada, przerywamy (graf niespójny)
        if (nextCity == -1) {
            cerr << "Nie znaleziono sąsiada" << endl;
            delete[] visited;
            delete[] path;
            return nullptr;
        }

        // Przejście do kolejnego miasta
        totalCost += minCost;
        currentCity = nextCity;
        visited[currentCity] = true;
        path[step] = currentCity;
    }

    // Powrót do miasta początkowego
    if (distanceMatrix[currentCity][0] != -1) {
        totalCost += distanceMatrix[currentCity][0];
        path[numVertices] = 0; // Zamykamy cykl
        cout << "ostatni element w path: " << path[numVertices] << endl;

    } else {
        cerr << "Brak połączenia powrotnego do miasta początkowego" << endl;
        delete[] visited;
        delete[] path;
        return nullptr;
    }

    // Wyświetlenie najlepszego rozwiązania
    cout << "Najlepsza trasa: ";
    for (int i = 0; i <= numVertices; ++i) {
        cout << path[i] << " ";
    }
    cout << endl;
    cout << "Minimalny koszt: " << totalCost << endl;

    delete[] visited;
    return path; // Zwracamy tablicę najlepszej trasy
}
