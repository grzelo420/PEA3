//
// Created by Jakub Grzegocki on 04.01.2024.
//

#include "Menu.h"
#include <iostream>
#include <limits>

Menu::Menu() {
    graph = new Graph();
    stop = 60; // Domyślna wartość 1 minuta
    wielkoscPopulacji = 100; // Domyślna wartość
    wspolczynnikMutacji = 0.01f; // Domyślna wartość
    wspolczynnikKrzyzowania = 0.7f; // Domyślna wartość
    uzyjPBX = true; // Domyślnie ustawiona metoda krzyżowania na PBX
    algorytm = nullptr;
    uzyjInwersji = true;
}

Menu::~Menu() {
    delete graph;
    if (algorytm != nullptr) {
        delete algorytm;
    }
}

void Menu::wyswietlMenu() {
    std::string wybor;
    while (true) {
        system("cls");
        std::cout<<"STOP:            "<<stop<<std::endl;
        std::cout<<"WIELK POPULACJI: "<<wielkoscPopulacji<<std::endl;
        std::cout<<"WSP MUTACJI:     "<<wspolczynnikMutacji<<std::endl;
        std::cout<<"WSP KRZYZ:       "<<wspolczynnikKrzyzowania<<std::endl;
        std::cout<<"Czy pbx:         "<<uzyjPBX<<std::endl;
        std::cout<<"================================================"<<std::endl;
        std::cout << "Menu:" << std::endl;
        std::cout << "1. Wczytanie danych z pliku" << std::endl;
        std::cout << "2. Wprowadzenie kryterium stopu" << std::endl;
        std::cout << "3. Ustawienie wielkosci populacji poczatkowej" << std::endl;
        std::cout << "4. Ustawienie wspolczynnika mutacji" << std::endl;
        std::cout << "5. Ustawienia wspolczynnika krzyzowania" << std::endl;
        std::cout << "6. Wybor metody krzyzowania" << std::endl;
        std::cout << "7. Wybor metody mutacji" << std::endl;
        std::cout << "8. Uruchomienie algorytmu" << std::endl;
        std::cout << "9. Wyjscie" << std::endl;
        std::cout << "Wybor: ";
        std::cin >> wybor;

        if (wybor == "1") {
            wczytajDane();
        } else if (wybor == "2") {
            ustawStop();
        } else if (wybor == "3") {
            ustawWielkoscPopulacji();
        } else if (wybor == "4") {
            ustawWspolczynnikMutacji();
        } else if (wybor == "5") {
            ustawWspolczynnikKrzyzowania();
        } else if (wybor == "6") {
            wybierzMetodeKrzyzowania();
        } else if (wybor == "7") {
            wybierzMetodeMutacji();
        } else if (wybor == "8") {
            uruchomAlgorytm();
        } else if (wybor == "9") {
            break;
        } else {
            std::cout << "Niepoprawna opcja. Sprobuj ponownie." << std::endl;
        }
    }
}

void Menu::wczytajDane() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Czyszczenie bufora
    int choice;
    std::string nazwaPliku[3];
    nazwaPliku[0] = "C:/Users/Jakub Grzegocki/Desktop/ftv170.atsp";
    nazwaPliku[1] = "C:/Users/Jakub Grzegocki/Desktop/ftv47.atsp";
    nazwaPliku[2] = "C:/Users/Jakub Grzegocki/Desktop/rbg403.atsp";

    cout<<"1) 170 2) 47 3)403"<<endl;
    std::cout << "wybierz plik: ";
    std::cin>>choice;

    if (graph->wczytajGraf(nazwaPliku[choice-1])) {
        std::cout << "Wczytano dane: " << std::endl;
        std::cout << graph->toString() << std::endl;

    } else {
        std::cout << "Nie udało się wczytać danych." << std::endl;

    }
    system("pause");
}

void Menu::ustawStop() {
    std::cout << "Podaj kryterium stopu (czas w sekundach): ";
    std::cin >> stop;
}

void Menu::ustawWielkoscPopulacji() {
    std::cout << "Podaj wielkosc populacji: ";
    std::cin >> wielkoscPopulacji;
}

void Menu::ustawWspolczynnikMutacji() {
    std::cout << "Podaj wspolczynnik mutacji (np. 0.01): ";
    std::cin >> wspolczynnikMutacji;
}

void Menu::ustawWspolczynnikKrzyzowania() {
    std::cout << "Podaj wspolczynnik krzyzowania (np. 0.7): ";
    std::cin >> wspolczynnikKrzyzowania;
}

void Menu::wybierzMetodeKrzyzowania() {
    int wybor;
    std::cout << "Wybierz metode krzyzowania (1 - PBX, 2 - OX): ";
    std::cin >> wybor;
    uzyjPBX = (wybor == 1);
}

void Menu::wybierzMetodeMutacji() {
    int wybor;
    std::cout << "Wybierz metode mutacji (1 - Swap, 2 - Inwersja): ";
    std::cin >> wybor;
    uzyjInwersji = (wybor == 2);
}

void Menu::uruchomAlgorytm() {
    algorytm = new AlgorytmGenetyczny(graph, stop, wielkoscPopulacji, wspolczynnikKrzyzowania, wspolczynnikMutacji);
    algorytm->wykonaj(uzyjPBX, uzyjInwersji);
}
