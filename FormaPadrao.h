#ifndef FORMAPADRAO_H
#define FORMAPADRAO_H

#include <vector>
#include "ProblemaPL.h"

using namespace std;

class FormaPadrao {
private:
    vector<vector<double>> matriz;
    vector<int> baseInicial;

    int qtdFolgasExcessos;
    int qtdArtificiais;
    int qtdColunas;

    void preencherMatriz(const ProblemaPL& problema);

public:
    FormaPadrao(const ProblemaPL& problema);

    const vector<vector<double>>& getMatriz() const;
    const vector<int>& getBaseInicial() const;

    int getQtdColunas() const;
    int getQtdFolgasExcessos() const;
    int getQtdArtificiais() const;

    void imprimir() const;
};

#endif