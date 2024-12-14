#ifndef UNTITLED1_TABUSEARCH_H
#define UNTITLED1_TABUSEARCH_H

class TabuSearch {
public:
    double calculateCost(int* tour, int numCities, double** distanceMatrix);
    void generateNeighbor(int* tour, int numCities);
    bool isTabu(int* candidate, int numCities, int** tabuList, int tenure, int currentIteration);
    void updateTabuList(int* solution, int numCities, int** tabuList, int tenure, int currentIteration);
    int* tabuSearch(double** distanceMatrix, int numCities, int maxIterations, int tabuTenure, int timeLimitMs);
};

#endif //UNTITLED1_TABUSEARCH_H
