#ifndef UNTITLED1_ALGORITHM_H
#define UNTITLED1_ALGORITHM_H

class Algorithm {
protected:
    double calculateCost(int* tour, int numCities, double** distanceMatrix);
    void generateNeighbor(int* tour, int numCities);
};

#endif //UNTITLED1_ALGORITHM_H
