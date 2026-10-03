#ifndef RESTRICAO_H
#define RESTRICAO_H

#include <vector>
#include <string>

using namespace std;

class Restricao {
private:
    vector<double> coeficientes;
    string operador;
    double ladoDireito;

public:
    Restricao(int qtdVariaveis);

    void ler();
    void corrigirLadoDireitoNegativo();

    const vector<double>& getCoeficientes() const;
    string getOperador() const;
    double getLadoDireito() const;
};

#endif