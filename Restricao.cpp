#include <iostream>
#include "Restricao.h"

using namespace std;

Restricao::Restricao(int qtdVariaveis) {
    coeficientes.resize(qtdVariaveis);
    ladoDireito = 0.0;
}

void Restricao::ler() {
    for (int i = 0; i < (int)coeficientes.size(); i++) {
        cin >> coeficientes[i];
    }

    cin >> operador;
    cin >> ladoDireito;
}

void Restricao::corrigirLadoDireitoNegativo() {
    if (ladoDireito < 0) {

        for (int i = 0; i < (int)coeficientes.size(); i++) {
            coeficientes[i] *= -1;
        }

        ladoDireito *= -1;

        if (operador == "<=") {
            operador = ">=";
        }
        else if (operador == ">=") {
            operador = "<=";
        }
    }
}

const vector<double>& Restricao::getCoeficientes() const {
    return coeficientes;
}

string Restricao::getOperador() const {
    return operador;
}

double Restricao::getLadoDireito() const {
    return ladoDireito;
}