//
// Created by Jakub Grzegocki on 04.01.2024.
//

#ifndef PEA3_RELEASE_MENU_H
#define PEA3_RELEASE_MENU_H

#include "Graph.h"
#include "AlgorytmGenetyczny.h"

class Menu {
private:
    Graph* graph;
    AlgorytmGenetyczny* algorytm;
    int stop;
    int wielkoscPopulacji;
    float wspolczynnikMutacji;
    float wspolczynnikKrzyzowania;
    bool uzyjPBX;
    bool uzyjInwersji;
public:
    Menu();
    ~Menu();
    void wyswietlMenu();
    void wczytajDane();
    void ustawStop();
    void ustawWielkoscPopulacji();
    void ustawWspolczynnikMutacji();
    void ustawWspolczynnikKrzyzowania();
    void wybierzMetodeKrzyzowania();
    void uruchomAlgorytm();
    void wybierzMetodeMutacji();
};

#endif //PEA3_RELEASE_MENU_H
