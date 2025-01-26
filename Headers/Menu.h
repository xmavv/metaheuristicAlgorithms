#define UNTITLED1_MENU_H

#ifndef PROJEKT_SORTOWANIE_MENU_H
#define PROJEKT_SORTOWANIE_MENU_H
#include "stdio.h"
#include "iostream"
#include "time.h"
#include "cstdlib"
#include "Utilities.h"
#include "SimulatedAnnealing.h"
#include "Greedy.h"
#include "TabuSearch.h"
#include "AntColony.h"

// colors
#define WHITE   7
#define GREEN   10
#define YELLOW  14
#define MAGENTA 13

using namespace std;

class Menu {
    Utilities u;
    int userChoice;
    double **incidenceMatrix;
    int arrayLength;
    int timeLimitMs;
    double mutationFactor;
    int initialPopulation;
    double crossoverFactor;
    int* currentPath;
    SimulatedAnnealing sa;
//    Greedy greedy;
//    TabuSearch ts;
    AntColony ac;

public:
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    Menu();
    void run();
    void test();
    void chooseArray();
    double chooseAlgorithm();
    void chooseStopCriterion();
    void chooseCoolingRate();
    void loadPathAndCalculate();
    void saveCurrentPath();
    void loadArrayFromFile(string name);
    void chooseInitialPopulation();
    void chooseCrossoverFactor();
    void chooseMutationFactor();
    void chooseCrossoverMethod();
};

#endif //UNTITLED1_MENU_H
