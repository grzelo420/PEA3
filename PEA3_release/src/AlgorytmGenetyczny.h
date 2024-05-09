//
// Created by Jakub Grzegocki on 04.01.2024.
//

#ifndef PEA3_RELEASE_ALGORYTMGENETYCZNY_H
#define PEA3_RELEASE_ALGORYTMGENETYCZNY_H

#include <random>
#include "Graph.h"

class AlgorytmGenetyczny {
private:
    Graph* graph;
    int stop;
    int populacja;
    int size;
    float crossRate;
    float mutationRate;
    float originalMutationRate;
    std::vector<std::vector<int>> populacje;
    std::mt19937 mt;

    static bool czyJestwSciezce(int element, const std::vector<int>& sciezka);
    void mutacja(std::vector<int>& sciezka);
    std::vector<int> PositionBasedCrossover(const std::vector<int>& rodzic1, const std::vector<int>& rodzic2);
    std::vector<int> OrderedCrossover(const std::vector<int>& rodzic1, const std::vector<int>& rodzic2);
    int obliczKosztSciezki(const std::vector<int>& sciezka);
    std::vector<int> rankSelection();

public:
    AlgorytmGenetyczny(Graph* graph, int stop, int populacja, float crossRate, float mutationRate);
    void wykonaj(bool uzyjPBX, bool uzyjInwersji);

    void mutacjaInwersyjna(vector<int> &sciezka);
};

#endif //PEA3_RELEASE_ALGORYTMGENETYCZNY_H
