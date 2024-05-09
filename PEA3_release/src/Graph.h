//
// Created by Jakub Grzegocki on 28.12.2023.
//

#include <string>
#include <vector>
#ifndef PEA_2_RELEASE_GRAPH_H
#define PEA_2_RELEASE_GRAPH_H

using namespace std;

class Graph {

private:
    string opis_macierzy = " ";
    int **macierz = nullptr;
    int licznik_miast = 0;

public:

    string zwrocOpis();
    string toString();
    bool wczytajGraf(string plik);
    int zwrocLicznik();
    int** zwrocMacierz();
    Graph();
    ~Graph();
};


#endif //PEA_2_RELEASE_GRAPH_H
