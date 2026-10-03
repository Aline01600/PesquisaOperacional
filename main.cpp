#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Restricao {
    
    private: 
    vector<double> coeficientes;
    string operador;
    double ladoDireito;

    public: 
    Restricao(int qtdVariaveis){
        coeficientes.resize(qtdVariaveis);
        ladoDireito = 0.0;
    } 

    void ler(){
        for(int i =  0; i < coeficientes.size();  i++){
            cin>> coeficientes[i];
        }
        cin >> operador;
        cin >> ladoDireito;
    }

    void corrigirLadoDireitoNegativo(){

        if (ladoDireito < 0){
            for(int i = 0; i < coeficientes.size(); i++){
                coeficientes[i]*= -1;
            }
            ladoDireito *= -1;

            if(operador == "<="){
                operador = ">=";
            }

             if(operador == ">="){
                operador = "<=";
            }
        }
    }
    //apenas para verificação da funão de leitura.
    void imprimir() const {
        for (double coeficiente : coeficientes) {
            cout << coeficiente << " ";
        }

        cout << operador << " ";
        cout << ladoDireito;
    }
};

class ProblemaPL {
    private:
        int qtdVariaveis;
        int qtdRestricoes;
        string tipoOtimizacao;

        vector<double> coeficientesObjetivo;
        vector<Restricao> restricoes;

        public: 
            void lerEntrada(){
                cin >> qtdVariaveis >> qtdRestricoes;
                cin >> tipoOtimizacao;

                coeficientesObjetivo.resize(qtdVariaveis);

                for (int i = 0; i < qtdVariaveis; i++){
                    cin >> coeficientesObjetivo[i];
                }

                restricoes.clear();
                restricoes.reserve(qtdRestricoes);

                for(int i = 0; i<qtdRestricoes; i++){
                    Restricao restricao(qtdVariaveis);
                    restricao.ler();
                    restricoes.push_back(restricao);
                }
                
            }

            void corrigirLadoDireitoNegativo() {
                for (int i = 0; i < qtdRestricoes; i++) {
                    restricoes[i].corrigirLadoDireitoNegativo();
                }
            }

            void imprimir() const {
                cout << "Qtd. variaveis = " << qtdVariaveis << '\n';
                cout << "Qtd. restricoes = " << qtdRestricoes << '\n';
                cout << "Tipo de otimizacao = " << tipoOtimizacao << '\n';

                cout << "Objetivo: ";

                for (double coeficiente : coeficientesObjetivo) {
                    cout << coeficiente << " ";
                }

                cout << '\n';

                for (int i = 0; i < qtdRestricoes; i++) {
                    cout << "Restricao " << i + 1 << ": ";

                    restricoes[i].imprimir();

                    cout << '\n';
                }
            }
};


int main() {

    ProblemaPL problema;

    problema.lerEntrada();
    problema.corrigirLadoDireitoNegativo();
    problema.imprimir();

    return 0;
}