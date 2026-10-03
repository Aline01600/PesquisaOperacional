#include <iostream>
#include "FormaPadrao.h"

using namespace std;

FormaPadrao::FormaPadrao(const ProblemaPL& problema) {

    qtdFolgasExcessos = 0;
    qtdArtificiais = 0;

    for (const Restricao& restricao : problema.getRestricoes()) {

        if (restricao.getOperador() == "<=") {
            qtdFolgasExcessos++;
        }

        else if (restricao.getOperador() == ">=") {
            qtdFolgasExcessos++;
            qtdArtificiais++;
        }

        else if (restricao.getOperador() == "=") {
            qtdArtificiais++;
        }
    }

    qtdColunas = problema.getQtdVariaveis() + qtdFolgasExcessos + qtdArtificiais + 1;

    matriz.resize(problema.getQtdRestricoes(), vector<double>(qtdColunas, 0.0));
    baseInicial.resize(problema.getQtdRestricoes());

    preencherMatriz(problema);
}

void FormaPadrao::preencherMatriz(const ProblemaPL& problema) {

    int colunaFolgaExcesso = problema.getQtdVariaveis();
    int colunaArtificial = problema.getQtdVariaveis() + qtdFolgasExcessos;

    for (int i = 0; i < problema.getQtdRestricoes(); i++) {

        const Restricao& restricao = problema.getRestricoes()[i];

        for (int j = 0; j < problema.getQtdVariaveis(); j++) {
            matriz[i][j] = restricao.getCoeficientes()[j];
        }

        if (restricao.getOperador() == "<=") {

            matriz[i][colunaFolgaExcesso] = 1.0;
            baseInicial[i] = colunaFolgaExcesso;

            colunaFolgaExcesso++;
        }

        else if (restricao.getOperador() == ">=") {

            matriz[i][colunaFolgaExcesso] = -1.0;
            matriz[i][colunaArtificial] = 1.0;

            baseInicial[i] = colunaArtificial;

            colunaFolgaExcesso++;
            colunaArtificial++;
        }

        else if (restricao.getOperador() == "=") {

            matriz[i][colunaArtificial] = 1.0;
            baseInicial[i] = colunaArtificial;

            colunaArtificial++;
        }

        matriz[i][qtdColunas - 1] = restricao.getLadoDireito();
    }
}

const vector<vector<double>>& FormaPadrao::getMatriz() const {
    return matriz;
}

const vector<int>& FormaPadrao::getBaseInicial() const {
    return baseInicial;
}

int FormaPadrao::getQtdColunas() const {
    return qtdColunas;
}

int FormaPadrao::getQtdFolgasExcessos() const {
    return qtdFolgasExcessos;
}

int FormaPadrao::getQtdArtificiais() const {
    return qtdArtificiais;
}

void FormaPadrao::imprimir() const {

    cout << "\nForma padrao:\n";

    for (const vector<double>& linha : matriz) {

        for (double valor : linha) {
            cout << valor << "\t";
        }

        cout << '\n';
    }
}