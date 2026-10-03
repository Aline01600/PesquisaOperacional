#include <iostream>
#include "ProblemaPL.h"

using namespace std;

void ProblemaPL::lerEntrada() {
    cin >> qtdVariaveis >> qtdRestricoes;
    cin >> tipoOtimizacao;

    coeficientesObjetivo.resize(qtdVariaveis);

    for (int i = 0; i < qtdVariaveis; i++) {
        cin >> coeficientesObjetivo[i];
    }

    restricoes.clear();
    restricoes.reserve(qtdRestricoes);

    for (int i = 0; i < qtdRestricoes; i++) {

        Restricao restricao(qtdVariaveis);

        restricao.ler();

        restricoes.push_back(restricao);
    }
}

void ProblemaPL::corrigirLadoDireitoNegativo() {
    for (int i = 0; i < qtdRestricoes; i++) {
        restricoes[i].corrigirLadoDireitoNegativo();
    }
}

int ProblemaPL::getQtdVariaveis() const {
    return qtdVariaveis;
}

int ProblemaPL::getQtdRestricoes() const {
    return qtdRestricoes;
}

string ProblemaPL::getTipoOtimizacao() const {
    return tipoOtimizacao;
}

const vector<double>& ProblemaPL::getCoeficientesObjetivo() const {
    return coeficientesObjetivo;
}

const vector<Restricao>& ProblemaPL::getRestricoes() const {
    return restricoes;
}