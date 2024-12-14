#include "../Headers/Greedy.h"


int* Greedy::greedyAlgorithm(double** distanceMatrix, int numVertices) {
    bool* visited = new bool[numVertices];
    int* path = new int[numVertices + 1];
    double totalCost = 0.0;

    for (int i = 0; i < numVertices; ++i) {
        visited[i] = false;
    }

    int currentCity = 0;
    visited[currentCity] = true;
    path[0] = currentCity;

    // idziemy po kolejnych miastach
    for (int step = 1; step < numVertices; ++step) {
        double minCost = numeric_limits<double>::max();
        int nextCity = -1;

        // bierzemy najtansze jeszcze nieodwiedzone miasto
        for (int city = 0; city < numVertices; ++city) {
            if (!visited[city] && distanceMatrix[currentCity][city] != -1) {
                if (distanceMatrix[currentCity][city] < minCost) {
                    minCost = distanceMatrix[currentCity][city];
                        nextCity = city;
                }
            }
        }

        if (nextCity == -1) {
            cerr << "Nie znaleziono sąsiada" << endl;
            delete[] visited;
            delete[] path;
            return nullptr;
        }

        totalCost += minCost;
        currentCity = nextCity;
        visited[currentCity] = true;
        path[step] = currentCity;
    }

    // powrot do miasta 0
    if (distanceMatrix[currentCity][0] != -1) {
        totalCost += distanceMatrix[currentCity][0];
        path[numVertices] = 0;
        cout << "ostatni element w path: " << path[numVertices] << endl;

    } else {
        cerr << "Brak połączenia powrotnego do miasta początkowego" << endl;
        delete[] visited;
        delete[] path;
        return nullptr;
    }

    cout << "Najlepsza trasa: ";
    for (int i = 0; i <= numVertices; ++i) {
        cout << path[i] << " ";
    }
    cout << endl;
    cout << "Minimalny koszt: " << totalCost << endl;

    delete[] visited;
    return path;
}
