//
// Created by Jakub Grzegocki on 28.12.2023.
//

#include <fstream>
#include <time.h>
#include <ctime>
#include <string>
#include <iostream>
#include <vector>
#include "Graph.h"

Graph::Graph() {}

Graph::~Graph() {}

int** Graph::zwrocMacierz() {
    return macierz;
}

int Graph::zwrocLicznik() {
    return licznik_miast;
}

string Graph::zwrocOpis() {
    if(licznik_miast){
        return opis_macierzy;
    }else{
        return "Nie wprowadzono macierzy \n";
    }
}

string Graph::toString() {

    string znaki = "";
    znaki.append(opis_macierzy);
    if (licznik_miast) {
        for (int i = 0; i < licznik_miast; i++) {
            for (int j = 0; j < licznik_miast; j++) {
                if ((macierz[i][j] < 10 and (macierz[i][j] >= 0)))
                    znaki += to_string(macierz[i][j]) + "    ";
                else if (((macierz[i][j] < 100 and (macierz[i][j] > 9)) or (macierz[i][j] < 0)))
                    znaki += to_string(macierz[i][j]) + "   ";
                else if (macierz[i][j] == 100000000)
                    znaki += "N   ";
                else
                    znaki += to_string(macierz[i][j]) + "  ";
            }
            znaki += "\n";
        }
    } else {
        znaki = "Macierz jest pusta\n";
    }
    return znaki;
}

bool Graph::wczytajGraf(string plik) {
    int **tym;
    opis_macierzy = "";
    string tymOpis = "";
    ifstream wejscie;
    wejscie.open(plik);

    if(wejscie.fail() or wejscie.eof())
    {
        return false;
    }

    if(licznik_miast){
        for(int i = 0; i < licznik_miast; i++){
            delete[] macierz[i];
        }

        delete[] macierz;
    }

    getline(wejscie, tymOpis);
    opis_macierzy.append(tymOpis + "\n");
    getline(wejscie, tymOpis);
    opis_macierzy.append(tymOpis + "\n");
    getline(wejscie, tymOpis);
    opis_macierzy.append(tymOpis + "\n");
    wejscie >> tymOpis;
    opis_macierzy.append(tymOpis);
    wejscie >> licznik_miast;
    opis_macierzy.append(to_string(licznik_miast) + "\n");
    getline(wejscie, tymOpis);
    getline(wejscie, tymOpis);
    getline(wejscie, tymOpis);
    getline(wejscie, tymOpis);

    tym = new int *[licznik_miast];

    for (int i = 0 ; i< licznik_miast; i++){
        tym[i] = new int[licznik_miast];
    }

    for(int i = 0; i < licznik_miast; i++){
        for(int j = 0; j < licznik_miast; j++){
            wejscie >> tym[i][j];
        }
    }

    wejscie.close();
    macierz = tym;
    return true;

}
