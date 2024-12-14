#ifndef UNTITLED1_TABUSEARCH_H
#define UNTITLED1_TABUSEARCH_H
#include "Algorithm.h."

class TabuSearch: public Algorithm {
public:
    bool isTabu(int* candidate, int numCities, int** tabuList, int tenure, int currentIteration);
    void updateTabuList(int* solution, int numCities, int** tabuList, int tenure, int currentIteration);
    int* tabuSearch(double** distanceMatrix, int numCities, int maxIterations, int tabuTenure, int timeLimitMs);
};

#endif //UNTITLED1_TABUSEARCH_H
