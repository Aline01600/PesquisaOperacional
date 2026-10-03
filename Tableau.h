#ifndef TABLEAU_H
#define TABLEAU_H

#include <vector>
#include "FormaPadrao.h"

using namespace std;

class Tableau {
private:
    vector<vector<double>> matriz;
    vector<int> base;

    int qtdLinhas;
    int qtdColunas;
    int linhaObjetivo;

    void copiarRestricoes(const FormaPadrao& formaPadrao, const ProblemaPL& problema);
    void ajustarLinhaObjetivo();

public:
    Tableau(const FormaPadrao& formaPadrao, const ProblemaPL& problema);

    void montarFase1(const FormaPadrao& formaPadrao, const ProblemaPL& problema);
    void montarFase2(const ProblemaPL& problema);

    vector<vector<double>>& getMatriz();
    const vector<vector<double>>& getMatriz() const;

    vector<int>& getBase();
    const vector<int>& getBase() const;

    int getQtdLinhas() const;
    int getQtdColunas() const;
    int getLinhaObjetivo() const;

    void imprimir() const;
};

#endif