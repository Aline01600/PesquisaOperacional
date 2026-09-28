#include <iostream>
#include <vector>
#include <string>


using namespace std;

struct Restricao {
    vector<double> coeficientes;
    string operador;
    double ladoDireito;
};

struct ProblemaPL {
    int qtdVariaveis;
    int qtdRestricoes;
    string tipoOtimizacao;

    vector<double> coeficientesObjetivo;
    vector<Restricao> restricoes;
};

struct FormaPadrao{
    vector<vector<double>>matriz;
}

ProblemaPL lerProblema() {
    ProblemaPL problema;

    cin >> problema.qtdVariaveis >> problema.qtdRestricoes;
    cin >> problema.tipoOtimizacao;

    problema.coeficientesObjetivo.resize(problema.qtdVariaveis);

    for (int i = 0; i < problema.qtdVariaveis; i++) {
        cin >> problema.coeficientesObjetivo[i];
    }

    problema.restricoes.resize(problema.qtdRestricoes);

    for (int i = 0; i < problema.qtdRestricoes; i++) {
        problema.restricoes[i].coeficientes.resize(problema.qtdVariaveis);

        for (int j = 0; j < problema.qtdVariaveis; j++) {
            cin >> problema.restricoes[i].coeficientes[j];
        }

        cin >> problema.restricoes[i].operador;
        cin >> problema.restricoes[i].ladoDireito;
    }

    return problema;
}

void normalizarProblema(ProblemaPL& problema) {

    //Lado direito negativo
    for (int i = 0; i < problema.qtdRestricoes; i++) {

        if (problema.restricoes[i].ladoDireito < 0) {

            for (int j = 0; j < problema.qtdVariaveis; j++) {
                problema.restricoes[i].coeficientes[j] *= -1;
            }

            problema.restricoes[i].ladoDireito *= -1;

            if (problema.restricoes[i].operador == "<=") {
                problema.restricoes[i].operador = ">=";
            }
            else if (problema.restricoes[i].operador == ">=") {
                problema.restricoes[i].operador = "<=";
            }
        }
    }

}

void formaPadrão(ProblemaPL& problema){
    //variaveis de folga e variaveis artificiais
    /*[1 2 3 1 0 0 0
       1 2 4 0-1 0 0
       1 4 2 0 0 1 0
       0 2 4 0 0 0 0 1]*/

   for (int i = 0; i < problema.qtdRestricoes; i++) {                                           
        if(problema.restricoes[i].operador == "<="){                                            
            problema.coeficientesObjetivo[i].push_back(porblema.restricoes[i].ladoDireito);                                    
                                                                                                
        }
        if(problema.restricoes[i].operador == ">="){
            problema.coeficientesObjetivo.push_back(-1.0);
            problema.coeficientesObjetivo.push_back(1.0);
        }
        if(problema.restricoes[i].operador == "=="){
            problema.coeficientesObjetivo.push_back(1.0);
        }
    }

}

void imprimirProblema(const ProblemaPL& problema) {
    cout << "Qtd. variaveis = " << problema.qtdVariaveis << '\n';
    cout << "Qtd. restricoes = " << problema.qtdRestricoes << '\n';
    cout << "Tipo de otimizacao = " << problema.tipoOtimizacao << '\n';
    cout << "Objetivo: ";

    for (int i = 0; i < problema.qtdVariaveis; i++) {
        cout << problema.coeficientesObjetivo[i] << " ";
    }

    cout << '\n';

    for (int i = 0; i < problema.qtdRestricoes; i++) {
        cout << "Restricao " << i + 1 << ": ";

        for (int j = 0; j < problema.qtdVariaveis; j++) {
            cout << problema.restricoes[i].coeficientes[j] << " ";
        }

        cout << problema.restricoes[i].operador << " ";
        cout << problema.restricoes[i].ladoDireito << '\n';
    }
}

int main() {

    ProblemaPL problema = lerProblema();
    normalizarProblema(problema);
    imprimirProblema(problema);

    return 0;
}