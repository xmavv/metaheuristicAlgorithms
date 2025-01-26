#include <vector>
#include "../Headers/Menu.h"
#include "iostream"
#include "fstream"

using namespace std;

Menu::Menu() {
//    this -> coolingRate = 0.999999;
    this -> timeLimitMs = 30000;
    this -> crossoverFactor = 0.8;
    this -> mutationFactor = 0.01;
    this -> initialPopulation = 2000;
}

void Menu::test() {
    loadArrayFromFile("ftv170.xml");

    for(int i=0; i<9; i++) {
//        ts.tabuSearch(incidenceMatrix, arrayLength, 9000000000000, arrayLength, timeLimitMs);
    }
}

void Menu::run() {
    chooseAlgorithm();
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
    ifstream file(name);
    if (!file) {
        cout << "nie można otworzyć pliku!" << endl;
    }

    vector<vector<double>> graph;
    int currentVertex = -1;
    string line;

    while (getline(file, line)) {
        // nowy wierzcholek znaleziony
        if (line.find("<vertex>") != string::npos) {
            currentVertex++;
            // dodajemy nowy wierszz
            graph.emplace_back();
        }

        // nowa krawedz w wierzcholku znaleziona
        if (line.find("<edge") != string::npos) {
            size_t costPos = line.find("cost=\"");
            //przesuwamy na koniec stringu cost="
            size_t endCostPos = line.find("\"", costPos + 6);
            size_t valueStart = line.find(">", endCostPos) + 1;
            size_t valueEnd = line.find("<", valueStart);

            // bierzemy koszt oraz numer wierzcholka
            string costStr = line.substr(costPos + 6, endCostPos - costPos - 6);
            string edgeValueStr = line.substr(valueStart, valueEnd - valueStart);

            double cost = stod(costStr);
            int edgeValue = stoi(edgeValueStr);

            if (graph[currentVertex].size() <= edgeValue) {
                graph[currentVertex].resize(edgeValue + 1, 0);
            }
            graph[currentVertex][edgeValue] = cost;
        }
    }

    int numVertices = graph.size();
    for (int i = 0; i < numVertices; ++i) {
        graph[i].resize(numVertices, 0);
        for (int j = 0; j < numVertices; ++j) {
            if (i == j) {
                graph[i][j] = -1;
            }
        }
    }

    arrayLength = numVertices;

    currentPath = new int[arrayLength];

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
}

void Menu::chooseStopCriterion() {
    Utilities::printColorText(hConsole,"podaj kryterium stopu",MAGENTA);
    cin >> timeLimitMs;
}

//void Menu::chooseCoolingRate() {
//    Utilities::printColorText(hConsole,"podaj wspolczynnik zmiany temperatury",MAGENTA);
//    cin >> coolingRate;
//}

void Menu::chooseInitialPopulation() {
    Utilities::printColorText(hConsole,"podaj poczatkowa populacje",MAGENTA);
    cin >> initialPopulation;
}

void Menu::chooseCrossoverFactor() {
    Utilities::printColorText(hConsole,"podaj wspolczynnik krzyzowania",MAGENTA);
    cin >> crossoverFactor;
}

void Menu::chooseMutationFactor() {
    Utilities::printColorText(hConsole,"podaj wspolczynnik mutacji",MAGENTA);
    cin >> mutationFactor;
}

void Menu::chooseCrossoverMethod() {
    Utilities::printColorText(hConsole,"wybierz metode krzyzowania: ",MAGENTA);
    Utilities::printColorText(hConsole,"PMX: 1",MAGENTA);
    Utilities::printColorText(hConsole,"OX: 2",MAGENTA);
    cin >> userChoice;
}

void Menu::saveCurrentPath() {
    ofstream myfile;
    myfile.open ("sciezka.txt");
    myfile << arrayLength<<endl;

    for(int i=0; i<arrayLength; i++) {
        myfile << currentPath[i]<<endl;
    }

    myfile << currentPath[0];

    myfile.close();
}

void Menu::loadPathAndCalculate() {
    string name;
    cout<< "podaj nazwe pliku: "<<endl;
    cin >> name;

    ifstream file(name);
    if(!file.is_open()) {
        cerr<<"nie udalo sie otworzyc pliku!"<<endl;
        exit(1);
    }

    file >> arrayLength;
    int* pathFromFile = new int[arrayLength+1];

    for (int i = 0; i < arrayLength+1; i++) {
        file >> pathFromFile[i];
    }

    file.close();

    double pathCost;
    for(int i=0; i<arrayLength; i++) {
        pathCost += incidenceMatrix[pathFromFile[i]][pathFromFile[i+1]];
    }

    cout << "obliczona droga: ";
    cout << pathCost<<endl;
}

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

        cout<<"9. ustaw wielkosc populacji poczatkowej"<<endl;
        cout<<"10. ustaw wspolczynnik mutacji"<<endl;
        cout<<"11. ustaw wspolczynnik krzyzowania"<<endl;
        cout<<"12. wybor metody krzyzowania"<<endl;

        cout<<"14. algorytm mrowkowy"<<endl;

        cin>>userChoice;

        switch (userChoice) {
            case 14:
                Utilities::printColorText(hConsole, "wybrales algorytm mrowkowy\n", GREEN);

                if(userChoice != 2) {
                    ac.runPMX(arrayLength, incidenceMatrix, initialPopulation, 2.0, 3.0, 0.7, crossoverFactor, mutationFactor, timeLimitMs);
                } else {
                    ac.runOX(arrayLength, incidenceMatrix, initialPopulation, 2.0, 3.0, 0.7, crossoverFactor, mutationFactor, timeLimitMs);
                }
                break;
            case 9:
                chooseInitialPopulation();
                break;
            case 10:
                chooseMutationFactor();
                break;
            case 11:
                chooseCrossoverFactor();
                break;
            case 12:
                chooseCrossoverMethod();
                break;
            case 6:
                Utilities::printColorText(hConsole, "wybrales simulated annealing\n", GREEN);

//                currentPath = sa.algorithm(incidenceMatrix,arrayLength,  coolingRate, 9000000000000, timeLimitMs);
                break;
            case 3:
                Utilities::printColorText(hConsole, "metoda greedy\n", GREEN);

//                currentPath = greedy.greedyAlgorithm(incidenceMatrix, arrayLength);
                break;
            case 4:
                Utilities::printColorText(hConsole, "wybrales tabu search\n", GREEN);

//                currentPath = ts.tabuSearch(incidenceMatrix, arrayLength, 9000000000000, arrayLength, timeLimitMs);
                break;
            case 5:
//                chooseCoolingRate();
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