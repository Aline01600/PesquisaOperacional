#include <iostream>
#include <iomanip>
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
    criarNomesColunas(problema);
}

void Tableau::copiarRestricoes(const FormaPadrao& formaPadrao, const ProblemaPL& problema) {

    const vector<vector<double>>& matrizForma = formaPadrao.getMatriz();

    for (int i = 0; i < problema.getQtdRestricoes(); i++) {

        for (int j = 0; j < qtdColunas; j++) {
            matriz[i][j] = matrizForma[i][j];
        }
    }
}

void Tableau::criarNomesColunas(const ProblemaPL& problema) {

    for (int j = 0; j < problema.getQtdVariaveis(); j++) {
        nomesColunas.push_back("x" + to_string(j + 1));
    }

    const vector<Restricao>& restricoes = problema.getRestricoes();

    for (int i = 0; i < (int)restricoes.size(); i++) {

        if (restricoes[i].getOperador() == "<=") {
            nomesColunas.push_back("s" + to_string(i + 1));
        }

        else if (restricoes[i].getOperador() == ">=") {
            nomesColunas.push_back("e" + to_string(i + 1));
        }
    }

    for (int i = 0; i < (int)restricoes.size(); i++) {

        if (restricoes[i].getOperador() == ">=" ||
            restricoes[i].getOperador() == "=") {

            nomesColunas.push_back("a" + to_string(i + 1));
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

    for (int j = inicioArtificiais; j < fimArtificiais; j++) {
        matriz[linhaObjetivo][j] = 0.0;
    }
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

void Tableau::removerLinha(int linha) {

    matriz.erase(matriz.begin() + linha);
    base.erase(base.begin() + linha);

    qtdLinhas--;
    linhaObjetivo = qtdLinhas - 1;
}

void Tableau::removerColunasArtificiais(int inicioArtificiais, int qtdArtificiais) {

    for (int i = 0; i < qtdLinhas; i++) {

        matriz[i].erase(
            matriz[i].begin() + inicioArtificiais,
            matriz[i].begin() + inicioArtificiais + qtdArtificiais
        );
    }

    nomesColunas.erase(
        nomesColunas.begin() + inicioArtificiais,
        nomesColunas.begin() + inicioArtificiais + qtdArtificiais
    );

    qtdColunas -= qtdArtificiais;
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

string Tableau::getNomeColuna(int coluna) const {

    if (coluna >= 0 && coluna < (int)nomesColunas.size()) {
        return nomesColunas[coluna];
    }

    return "?";
}

void Tableau::imprimir() const {

    cout << fixed << setprecision(4);

    cout << left << setw(10) << "BASE";

    for (const string& nome : nomesColunas) {
        cout << right << setw(12) << nome;
    }

    cout << right << setw(12) << "RHS" << '\n';

    for (int i = 0; i < qtdLinhas - 1; i++) {

        cout << left << setw(10) << getNomeColuna(base[i]);

        for (int j = 0; j < qtdColunas; j++) {

            double valor = matriz[i][j];

            if (abs(valor) < EPS) {
                valor = 0.0;
            }

            cout << right << setw(12) << valor;
        }

        cout << '\n';
    }

    cout << left << setw(10) << "Z";

    for (int j = 0; j < qtdColunas; j++) {

        double valor = matriz[linhaObjetivo][j];

        if (abs(valor) < EPS) {
            valor = 0.0;
        }

        cout << right << setw(12) << valor;
    }

    cout << "\n\n";
}