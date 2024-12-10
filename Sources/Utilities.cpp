#include "stdio.h"
#include "iostream"
#include "time.h"
#include "cstdlib"
#include "windows.h"
#include <iostream>
#include <chrono>
#include "../Headers/Utilities.h";
#include "../Headers/Utilities.h"
#include "iostream"
#include "windows.h"
#include <chrono>

using namespace std;
using namespace std::chrono;

// colors
#define WHITE   7
#define RED     12
#define GREEN   10
#define YELLOW  14
#define BLUE    9
#define MAGENTA 13
#define CYAN    11

using namespace std;

void Utilities::printArray(int **arr, int len, string mes){
    for (int i = 0; i < len; ++i) {
        for (int j = 0; j < len; ++j) {
            cout << arr[i][j] << " ";
        }
        cout << endl;
    }

    cout<<"nacisnij dowolny klawisz aby kontynuowac..."<<endl;
    cin.ignore();
    cin.get();
}

time_point<high_resolution_clock> Utilities::startTimer() {
    return high_resolution_clock::now();
}

double Utilities::getElapsedTime(time_point<high_resolution_clock> start) {
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
    return duration.count();
}

void Utilities::printColorText(HANDLE console, string message, int color) {
    SetConsoleTextAttribute(console, color);
    cout<<endl<<message<<endl<<endl;
    SetConsoleTextAttribute(console, WHITE);
}