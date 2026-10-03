#include <iostream>
#include <cmath>
#include "Tableau.h"

using namespace std;

const double EPS = 1e-9;

Tableau::Tableau(const FormaPadrao& formaPadrao, const ProblemaPL& problema) {

    qtdLinhas = problema.getQtdRestricoes() + 1;
    qtdColunas = formaPadrao.getQtdColunas();
    linhaObjetivo = qtdLinhas - 1;

    matriz.resize(qtdLinhas, vector<double>(qtdColunas, 0.0));

    base = formaPadrao.getBaseInicial();

    copiarRestricoes(formaPadrao, problema);
}

void Tableau::copiarRestricoes(const FormaPadrao& formaPadrao, const ProblemaPL& problema) {

    const vector<vector<double>>& matrizForma = formaPadrao.getMatriz();

    for (int i = 0; i < problema.getQtdRestricoes(); i++) {

        for (int j = 0; j < qtdColunas; j++) {
            matriz[i][j] = matrizForma[i][j];
        }
    }
}

void Tableau::ajustarLinhaObjetivo() {

    for (int i = 0; i < (int)base.size(); i++) {

        int colunaBasica = base[i];

        double coeficiente = matriz[linhaObjetivo][colunaBasica];

        if (abs(coeficiente) > EPS) {

            for (int j = 0; j < qtdColunas; j++) {
                matriz[linhaObjetivo][j] -= coeficiente * matriz[i][j];
            }
        }
    }
}

void Tableau::montarFase1(const FormaPadrao& formaPadrao, const ProblemaPL& problema) {

    for (int j = 0; j < qtdColunas; j++) {
        matriz[linhaObjetivo][j] = 0.0;
    }

    int inicioArtificiais = problema.getQtdVariaveis() + formaPadrao.getQtdFolgasExcessos();
    int fimArtificiais = inicioArtificiais + formaPadrao.getQtdArtificiais();

    for (int j = inicioArtificiais; j < fimArtificiais; j++) {
        matriz[linhaObjetivo][j] = 1.0;
    }

    ajustarLinhaObjetivo();
}

void Tableau::montarFase2(const ProblemaPL& problema) {

    for (int j = 0; j < qtdColunas; j++) {
        matriz[linhaObjetivo][j] = 0.0;
    }

    const vector<double>& objetivo = problema.getCoeficientesObjetivo();

    for (int j = 0; j < problema.getQtdVariaveis(); j++) {

        if (problema.getTipoOtimizacao() == "MAX") {
            matriz[linhaObjetivo][j] = -objetivo[j];
        }

        else {
            matriz[linhaObjetivo][j] = objetivo[j];
        }
    }

    ajustarLinhaObjetivo();
}

vector<vector<double>>& Tableau::getMatriz() {
    return matriz;
}

const vector<vector<double>>& Tableau::getMatriz() const {
    return matriz;
}

vector<int>& Tableau::getBase() {
    return base;
}

const vector<int>& Tableau::getBase() const {
    return base;
}

int Tableau::getQtdLinhas() const {
    return qtdLinhas;
}

int Tableau::getQtdColunas() const {
    return qtdColunas;
}

int Tableau::getLinhaObjetivo() const {
    return linhaObjetivo;
}

void Tableau::imprimir() const {

    cout << "\nTableau:\n";

    for (int i = 0; i < qtdLinhas; i++) {

        for (int j = 0; j < qtdColunas; j++) {
            cout << matriz[i][j] << "\t";
        }

        cout << '\n';
    }
}