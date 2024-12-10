#include "../Headers/Greedy.h"


GreedyResult Greedy::greedyAlgorithm(double** distanceMatrix, int numVertices) {
    vector<bool> visited(numVertices, false); // Oznaczenie, czy miasto zostało odwiedzone
    vector<int> path;                        // Trasa zachłanna
    double totalCost = 0.0;                  // Całkowity koszt trasy

    int currentCity = 0; // Zaczynamy od miasta 0
    visited[currentCity] = true;
    path.push_back(currentCity);

    // Znajdowanie kolejnych miast
    for (int step = 0; step < numVertices - 1; ++step) {
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
            cerr << "nie znaleziono sasiada" << endl;
            return { {}, -1.0 };
        }

        // Przejście do kolejnego miasta
        totalCost += minCost;
        currentCity = nextCity;
        visited[currentCity] = true;
        path.push_back(currentCity);
    }

    // Powrót do miasta początkowego
    if (distanceMatrix[currentCity][0] != -1) {
        totalCost += distanceMatrix[currentCity][0];
        path.push_back(0); // Zamykamy cykl
    } else {
        return { {}, -1.0 };
    }

    // Wyświetlenie najlepszego rozwiązania
    cout << "najlepsza trasa: ";
    for (int i = 0; i < numVertices; ++i) {
        cout << path[i] << " ";
    }
    cout << endl;
    cout << "minimalny koszt: " << totalCost << endl;

    return { path, totalCost };
}