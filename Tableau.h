#ifndef TABLEAU_H
#define TABLEAU_H

#include <vector>
#include <string>
#include "FormaPadrao.h"

using namespace std;

class Tableau {
private:
    vector<vector<double>> matriz;
    vector<int> base;
    vector<string> nomesColunas;

    int qtdLinhas;
    int qtdColunas;
    int linhaObjetivo;

    void copiarRestricoes(const FormaPadrao& formaPadrao, const ProblemaPL& problema);
    void ajustarLinhaObjetivo();
    void criarNomesColunas(const ProblemaPL& problema);
public:
    Tableau(const FormaPadrao& formaPadrao, const ProblemaPL& problema);

    void montarFase1(const FormaPadrao& formaPadrao, const ProblemaPL& problema);
    void montarFase2(const ProblemaPL& problema);

    void removerLinha(int linha);
    void removerColunasArtificiais(int inicioArtificiais, int qtdArtificiais);

    vector<vector<double>>& getMatriz();
    const vector<vector<double>>& getMatriz() const;

    vector<int>& getBase();
    const vector<int>& getBase() const;

    int getQtdLinhas() const;
    int getQtdColunas() const;
    int getLinhaObjetivo() const;

    string getNomeColuna(int coluna) const;

    void imprimir() const;
};

#endif