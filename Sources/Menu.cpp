#include <vector>
#include "../Headers/Menu.h"
#include "iostream"
#include "time.h"
#include "cstdlib"
#include "fstream"

using namespace std;

Menu::Menu() {
}

void Menu::run() {
//    int choice;
    this -> coolingRate = 0.9;
    this -> timeLimitMs = 120000;
    chooseAlgorithm();

//    while(true) {
//        cout << "witaj w projekcie ASTP! poruszając się po menu korzystaj tylko z cyfr!" << endl << endl;
//        cout << endl << "zacznij lub opusc aplikacje" << endl;
//        cout << "1. start" << endl;
//        cout<<"lub: "<<endl<<endl;
//        cout << "0. wyjdz z programu" << endl;
//        cin>>choice;
//
//        if(choice == 1) {
//            this -> chooseArray();
//        } else {
//            cerr << "nieprawidlowy wybor!" << endl;
//        }
//    }
}

void Menu::chooseArray() {
    string fileName;

    while(true) {
        userChoice = 2;

        Utilities::printColorText(hConsole,
                                  "podaj nazwe pliku (plik tekstowy musi znajodwac sie w tym samym folderze co plik wykonywalny programu!) ",
                                  MAGENTA);
        cout << "podaj nazwe pliku";
        cin >> fileName;
        loadArrayFromFile(fileName);

        chooseAlgorithm();
    }
}

void Menu::loadArrayFromFile(string name) {
    ifstream file(name); // Zmień nazwę pliku na właściwą
    if (!file) {
        cout << "nie można otworzyć pliku!" << endl;
    }

    // Inicjalizacja zmiennych
    vector<vector<double>> graph; // Macierz sąsiedztwa (dynamiczna)
    int currentVertex = -1; // Indeks bieżącego wierzchołka
    string line;

    // Przetwarzanie pliku linia po linii
    while (getline(file, line)) {
        // Jeśli znajdziemy nowy wierzchołek
        if (line.find("<vertex>") != string::npos) {
            currentVertex++; // Zwiększamy indeks wierzchołka
            // Dodajemy nowy wiersz do macierzy
            graph.emplace_back();
        }

        // Jeśli znajdziemy krawędź (edge)
        if (line.find("<edge") != string::npos) {
            size_t costPos = line.find("cost=\"");
            size_t endCostPos = line.find("\"", costPos + 6);
            size_t valueStart = line.find(">", endCostPos) + 1;
            size_t valueEnd = line.find("<", valueStart);

            // Pobieramy wartości kosztu i numeru wierzchołka
            string costStr = line.substr(costPos + 6, endCostPos - costPos - 6);
            string edgeValueStr = line.substr(valueStart, valueEnd - valueStart);

            double cost = stod(costStr);        // Konwertujemy koszt na liczbę
            int edgeValue = stoi(edgeValueStr); // Numer wierzchołka

            // Uzupełniamy wiersz macierzy
            if (graph[currentVertex].size() <= edgeValue) {
                graph[currentVertex].resize(edgeValue + 1, 0); // Rozszerzamy wiersz
            }
            graph[currentVertex][edgeValue] = cost;
        }
    }

    // Ustawianie -1 na diagonalach i wypełnienie brakujących kolumn
    int numVertices = graph.size();
    for (int i = 0; i < numVertices; ++i) {
        graph[i].resize(numVertices, 0); // Upewniamy się, że każda kolumna ma odpowiedni rozmiar
        for (int j = 0; j < numVertices; ++j) {
            if (i == j) {
                graph[i][j] = -1; // Diagonale ustawiamy na -1
            }
        }
    }

    arrayLength = numVertices;

    incidenceMatrix = new double*[arrayLength];
    for (int i = 0; i < arrayLength; ++i) {
        incidenceMatrix[i] = new double[arrayLength];
    }

    for (int i = 0; i < numVertices; ++i) {
        for (int j = 0; j < numVertices; ++j) {
            incidenceMatrix[i][j] = graph[i][j]; // Konwersja double -> int
        }
    }

    file.close();

    cout<<"czy chcesz zobaczyc wczytana macierz?"<<endl;
    cout<<"1. tak"<<endl;
    cout<<"2. nie"<<endl;
    cin>>userChoice;

    if(userChoice == 1) {
        for (int i = 0; i < numVertices; ++i) {
            for (int j = 0; j < numVertices; ++j) {
                cout << incidenceMatrix[i][j] << " ";
            }
            cout << endl;
        }
    }
}

void Menu::chooseStopCriterion() {
    Utilities::printColorText(hConsole,"podaj kryterium stopu",MAGENTA);
    cin >> timeLimitMs;
}

void Menu::chooseCoolingRate() {
    Utilities::printColorText(hConsole,"podaj wspolczynnik zmiany temperatury",MAGENTA);
    cin >> coolingRate;
}

void Menu::saveCurrentPath() {

    chooseAlgorithm();
}

void Menu::loadPathAndCalculate() {

    chooseAlgorithm();
};

double Menu::chooseAlgorithm() {

    while(userChoice != 0) {
        cout << "witaj w projekcie ASTP! poruszając się po menu korzystaj tylko z cyfr!" << endl << endl;
        cout << "--------------------MENU-1--------------------" << endl << endl;
        cout<<"wybierz opcje:"<<endl<<endl;

        cout<<"1. wczytaj dane z pliku"<<endl;
        cout<<"2. ustaw kryterium stopu"<<endl;
        cout<<"3. obliczenie metoda zachlanna"<<endl;

        cout<<"4. tabuSearch"<<endl;

        cout<<"5. ustaw wspolczynnik zmiany temperatury"<<endl;
        cout<<"6. algorytm simulatedAnnealing"<<endl;

        cout<<"7. zapisz sciezke rozwiazania"<<endl;
        cout<<"8. wczytaj sciezke i oblicz droge"<<endl;

//        cout<<"lub: "<<endl<<endl;
//        cout<<"0. wyjdz z programu"<<endl;
        cin>>userChoice;


        cout << userChoice<<endl;
        cout<< coolingRate<<endl;
//        if(userChoice == 9) {
//            break;
//        }

        switch (userChoice) {
            case 6:
                Utilities::printColorText(hConsole, "wybrales simulated annealing\n", GREEN);

//                sa.algorithm(incidenceMatrix,arrayLength,  0.999999, 9000000000000, 120000); 2min
                sa.algorithm(incidenceMatrix,arrayLength,  coolingRate, 9000000000000, timeLimitMs);

                break;
            case 3:
                Utilities::printColorText(hConsole, "metoda greedy\n", GREEN);

                greedy.greedyAlgorithm(incidenceMatrix, arrayLength);

                break;
            case 4:
//                Utilities::printColorText(hConsole, "dynamic programming\n", GREEN);

//                dp->tsp(incidenceMatrix, arrayLength);

                break;
            case 5:
                chooseCoolingRate();
                break;
            case 2:
                chooseStopCriterion();
                break;
            case 7:
                saveCurrentPath();
                break;
            case 8:
                loadPathAndCalculate();
                break;
            case 1:
                chooseArray();
                break;
            case 0:
                break;
            default:
                cerr<<"niepoprawny wybor!"<<endl;
                break;
        }
    }
    return 2;
}