#ifndef PROBLEMAPL_H
#define PROBLEMAPL_H

#include <vector>
#include <string>
#include "Restricao.h"

using namespace std;

class ProblemaPL {
private:
    int qtdVariaveis;
    int qtdRestricoes;
    string tipoOtimizacao;

    vector<double> coeficientesObjetivo;
    vector<Restricao> restricoes;

public:
    void lerEntrada();
    void corrigirLadoDireitoNegativo();

    int getQtdVariaveis() const;
    int getQtdRestricoes() const;
    string getTipoOtimizacao() const;

    const vector<double>& getCoeficientesObjetivo() const;
    const vector<Restricao>& getRestricoes() const;
};

#endif