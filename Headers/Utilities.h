#ifndef UNTITLED1_UTILITIES_H
#define UNTITLED1_UTILITIES_H

#include "stdio.h"
#include "iostream"
#include "time.h"
#include "cstdlib"
#include "windows.h"
#include <iostream>
#include <chrono>

// colors
#define WHITE   7
#define RED     12
#define GREEN   10
#define YELLOW  14
#define BLUE    9
#define MAGENTA 13
#define CYAN    11

using namespace std;

class Utilities {
public:
    Utilities() {};
    static void printArray(int **arr, int len, string mes);
    static void printColorText(HANDLE console, string message, int color);
    static std::chrono::time_point<std::chrono::high_resolution_clock> startTimer();
    static double getElapsedTime(std::chrono::time_point<std::chrono::high_resolution_clock> start);
};


#endif //UNTITLED1_UTILITIES_H
