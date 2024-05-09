//
// Created by Jakub Grzegocki on 04.01.2024.
//

#include "AlgorytmGenetyczny.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>

// lista inicjalizacyjna

AlgorytmGenetyczny::AlgorytmGenetyczny(Graph* graph, int stop, int populacja, float crossRate, float mutationRate) {
    this->graph = graph;
    this->stop = stop;
    this->populacja = populacja;
    this->crossRate = crossRate;
    this->mutationRate = mutationRate;
    this->originalMutationRate = mutationRate;
    this->size = graph->zwrocLicznik();

    // Inicjalizacja generatora liczb pseudolosowych
    std::random_device rd;
    mt = std::mt19937(rd());

    // Inicjalizacja początkowej populacji
    for (int i = 0; i < populacja; ++i) {
        std::vector<int> sciezka(graph->zwrocLicznik());
        std::iota(sciezka.begin(), sciezka.end(), 0); // Wypełnienie ścieżki wartościami 0, 1, 2, ..., n-1
        std::shuffle(sciezka.begin(), sciezka.end(), mt); // Losowe przetasowanie ścieżki
        populacje.push_back(sciezka);
    }
}

bool AlgorytmGenetyczny::czyJestwSciezce(int element, const std::vector<int>& sciezka) {
    return std::find(sciezka.begin(), sciezka.end(), element) != sciezka.end();
}

void AlgorytmGenetyczny::mutacja(std::vector<int>& sciezka) {
    std::uniform_int_distribution<int> dist(0, sciezka.size() - 1);
    int idx1 = dist(mt);
    int idx2 = dist(mt);
    std::swap(sciezka[idx1], sciezka[idx2]);
}

void AlgorytmGenetyczny::mutacjaInwersyjna(std::vector<int>& sciezka) {
    if (sciezka.size() < 2) return; // Nie ma sensu inwertować, jeśli jest tylko jeden element lub mniej

    std::uniform_int_distribution<int> dist(0, sciezka.size() - 1);
    int idx1 = dist(mt);
    int idx2 = dist(mt);

    // Upewnij się, że idx1 < idx2
    if (idx1 > idx2) {
        std::swap(idx1, idx2);
    }

    // Inwersja elementów między idx1 a idx2
    std::reverse(sciezka.begin() + idx1, sciezka.begin() + idx2 + 1);
}

std::vector<int> AlgorytmGenetyczny::PositionBasedCrossover(const std::vector<int>& rodzic1, const std::vector<int>& rodzic2) {
    std::vector<int> potomek(rodzic1.size(), -1); // Inicjalizacja potomka z wartościami -1
    std::uniform_int_distribution<int> dist(0, rodzic1.size() - 1);
    std::vector<bool> wybranePozycje(rodzic1.size(), false);

    // Wybór pozycji do zachowania z rodzica 1
    for (int i = 0; i < rodzic1.size() / 2; ++i) {
        int pozycja = dist(mt);
        potomek[pozycja] = rodzic1[pozycja];
        wybranePozycje[pozycja] = true;
    }

    // Uzupełnienie potomka wartościami z rodzica 2
    int idxRodzic2 = 0;
    for (int i = 0; i < potomek.size(); ++i) {
        if (potomek[i] == -1) {
            // Szukaj pierwszego nie wybranego miasta z rodzica 2
            while (czyJestwSciezce(rodzic2[idxRodzic2], potomek)) {
                ++idxRodzic2;
            }
            potomek[i] = rodzic2[idxRodzic2];
        }
    }
    return potomek;
}

std::vector<int> AlgorytmGenetyczny::OrderedCrossover(const std::vector<int>& rodzic1, const std::vector<int>& rodzic2) {

    int rozmiar = rodzic1.size();

    // Inicjalizacja potomka z wartościami -1
    std::vector<int> potomek(rozmiar, -1);
    int poczatek, koniec;

    // Losowe wybranie segmentu z pierwszego rodzica
    do {
        std::uniform_int_distribution<int> dist(0, rozmiar - 1);
        poczatek = dist(mt);
        koniec = dist(mt);
        if (poczatek > koniec) {std::swap(poczatek, koniec);}
    }while((0 >= (koniec - poczatek)) || !poczatek || !(koniec - (rozmiar - 1)));

    // Skopiowanie segmentu od pierwszego rodzica do potomka
    for (int i = poczatek; i <= koniec; ++i) {
        potomek[i] = rodzic1[i];
    }

    // Wypełnienie reszty potomka elementami z drugiego rodzica
    int idxPotomek = koniec + 1, idxRodzic2 = koniec + 1;
    do {
        if (!czyJestwSciezce(rodzic2[idxRodzic2 % rozmiar], potomek)) {
            potomek[idxPotomek % rozmiar] = rodzic2[idxRodzic2 % rozmiar];
            idxPotomek++;
        }
        idxRodzic2++;
    } while (idxPotomek % rozmiar != poczatek);


    return potomek;
}


std::vector<int> AlgorytmGenetyczny::rankSelection() {
    // Obliczanie fitness dla każdej ścieżki
    std::vector<int> fitness_values(populacja);
    for (int i = 0; i < populacja; ++i) {
        fitness_values[i] = obliczKosztSciezki(populacje[i]);
    }

    // Sortowanie indeksów ścieżek na podstawie fitness (odnajlepszego do najgorszego)
    std::vector<int> indeksy(populacja);
    std::iota(indeksy.begin(), indeksy.end(), 0);
    std::sort(indeksy.begin(), indeksy.end(), [&fitness_values](int i1, int i2) {
        return
                fitness_values[i1] < fitness_values[i2];
    });

    // Przypisanie rangi i obliczenie prawdopodobieństwa selekcji na podstawie rangi
    std::vector<double> prawdopodobienstwa(populacja);
    int suma_rang = populacja * (populacja + 1) / 2;
    for (int i = 0; i < populacja; ++i) {
        int ranga = i + 1; // Rangi zaczynają się od 1
        prawdopodobienstwa[indeksy[i]] = static_cast<double>(ranga) / suma_rang;
    }

    // Selekcja na podstawie wylosowanych prawdopodobieństw
    std::vector<int> wybraniRodzice;
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    for (int i = 0; i < populacja; ++i) {
        double los = dist(mt);
        double suma = 0.0;
        for (int j = 0; j < populacja; ++j) {
            suma += prawdopodobienstwa[j];
            if (los <= suma) {
                wybraniRodzice.push_back(j);
                break;
            }
        }
    }
//    cout<<endl<<endl;
    for(int i = 0; i < 100 ; i ++ ){
   // cout<<wybraniRodzice[i]<<" ";
   }
    return wybraniRodzice;
}

int AlgorytmGenetyczny::obliczKosztSciezki(const std::vector<int>& sciezka) {
    int koszt = 0;
    for (size_t i = 0; i < sciezka.size() - 1; ++i) {
        koszt += graph->zwrocMacierz()[sciezka[i]][sciezka[i + 1]];
    }
    koszt += graph->zwrocMacierz()[sciezka.back()][sciezka.front()];
    return koszt;
}

void AlgorytmGenetyczny::wykonaj(bool uzyjPBX, bool uzyjInwersji) {

    std::uniform_real_distribution<double> distDouble(0.0, 1.0);

    auto start = std::chrono::high_resolution_clock::now();
    int najlepszyKoszt = std::numeric_limits<int>::max();
    int stagnationCounter = 0; // Licznik stagnacji - ile generacji nie było poprawy

    std::vector<int> potomek, rodzic1, rodzic2, najlepszaSciezka, indeksyRodzicow;
    std::vector<std::vector<int>> nowaPopulacja;
    std::vector<std::pair<int, std::vector<int>>> combinedPopulacja;

    potomek.reserve(size); // Zakładając, że size to maksymalny rozmiar ścieżki
    rodzic1.reserve(size);
    rodzic2.reserve(size);
    nowaPopulacja.reserve(populacja);
    int endel = 1;
    bool improvement = false;
    std::cout<<endl<< "NEW BESTS: ";
    while (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - start).count() < stop) {
        nowaPopulacja.clear();
        nowaPopulacja.reserve(populacja);
        //cout<<"Wielkosc populcji: "<<populacja<<endl;
        improvement = false; // Flaga śledząca, czy była poprawa w tej generacji

        // Znajdowanie najlepszego osobnika w obecnej generacji
        for (const auto& sciezka : populacje) {
            int kosztSciezki = obliczKosztSciezki(sciezka);
            if (kosztSciezki < najlepszyKoszt) {
                najlepszyKoszt = kosztSciezki;
                std::cout << najlepszyKoszt << ", ";
//                std::cout << "NEW BEST: "
//                if(!(endel%10)){
//                std::cout<<endl<< "NEW BESTS: ";}

                najlepszaSciezka = sciezka;
                improvement = true;
            }
        }

        // Jeśli nie było poprawy, inkrementuj licznik stagnacji
        if (!improvement) {
            stagnationCounter++;
        } else {
            stagnationCounter = 0;
        }

        // Warunek dla hiper-mutacji
        if (stagnationCounter >= 10000) { // Jeśli nie było poprawy przez 50 generacji
//            std::cout << "R" << std::endl;
            mutationRate = 0.5; // Zwiększ współczynnik mutacji
        } else {
            mutationRate = originalMutationRate; // Powrót do oryginalnego współczynnika mutacji
        }

        if (improvement) {
            mutationRate = originalMutationRate; // Reset współczynnika mutacji do wartości początkowej
//          std::cout << "P" << std::endl;
        }

        // Dodanie najlepszego osobnika do nowej populacji
        nowaPopulacja.push_back(najlepszaSciezka);
        indeksyRodzicow = rankSelection();

        // Tworzenie reszty nowej populacji
        for (int i = 1; i < populacja; i++) {
            rodzic1 = populacje[indeksyRodzicow[i % populacja]];
            rodzic2 = populacje[indeksyRodzicow[(i + 1) % populacja]];
//            cout<<i<<" ";
//            cout<<"sprawdz: "<<sprawdz<<endl;
//             Sprawdzenie, czy należy przeprowadzić krzyżowanie
            if (distDouble(mt) < crossRate) {

                if (uzyjPBX) {
                    potomek = PositionBasedCrossover(rodzic1, rodzic2);
                } else {
                    potomek = OrderedCrossover(rodzic1, rodzic2);
                }
            } else {potomek = rodzic1;}

//             Sprawdzenie, czy należy przeprowadzić mutację
            if (distDouble(mt) < mutationRate) {

                if(uzyjInwersji){
                    mutacjaInwersyjna(potomek);
                }else{
                    mutacja(potomek);
                }
            }

            nowaPopulacja.push_back(potomek);
//            cout<<nowaPopulacja.size()<<" ";
        }
//        cout<<endl<<endl;

//        populacje = nowaPopulacja;
        for (const auto& sciezka : populacje) {
            combinedPopulacja.emplace_back(obliczKosztSciezki(sciezka), sciezka);
        }
        for (const auto& sciezka : nowaPopulacja) {
            combinedPopulacja.emplace_back(obliczKosztSciezki(sciezka), sciezka);
        }

        // Sortowanie połączonej populacji według kosztu ścieżek
        std::sort(combinedPopulacja.begin(), combinedPopulacja.end());

        // Usuwanie duplikatów ścieżek
        combinedPopulacja.erase(std::unique(combinedPopulacja.begin(), combinedPopulacja.end()), combinedPopulacja.end());

        // Wybór najlepszych ścieżek
        populacje.clear();
        for (int i = 0; i < populacja && i < combinedPopulacja.size(); ++i) {
            populacje.push_back(combinedPopulacja[i].second);
        }

    }

    // Wyświetlanie najlepszego wyniku
    std::cout << "Najlepsza znaleziona sciezka ma koszt: " << najlepszyKoszt << std::endl;
    for (int miasto : najlepszaSciezka) {
        std::cout << miasto << " ";
    }
    std::cout << std::endl;
    system("pause");
}
