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
    Restricao(int qtdVariaveis) {
        coeficientes.resize(qtdVariaveis);
        ladoDireito = 0.0;
    }

    void ler() {
        for (int i = 0; i < coeficientes.size(); i++) {
            cin >> coeficientes[i];
        }

        cin >> operador;
        cin >> ladoDireito;
    }

    void corrigirLadoDireitoNegativo() {
        if (ladoDireito < 0) {

            for (int i = 0; i < coeficientes.size(); i++) {
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

    const vector<double>& getCoeficientes() const {
        return coeficientes;
    }

    string getOperador() const {
        return operador;
    }

    double getLadoDireito() const {
        return ladoDireito;
    }

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
    void lerEntrada() {
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

    void corrigirLadoDireitoNegativo() {
        for (int i = 0; i < qtdRestricoes; i++) {
            restricoes[i].corrigirLadoDireitoNegativo();
        }
    }

    int getQtdVariaveis() const {
        return qtdVariaveis;
    }

    int getQtdRestricoes() const {
        return qtdRestricoes;
    }

    string getTipoOtimizacao() const {
        return tipoOtimizacao;
    }

    const vector<double>& getCoeficientesObjetivo() const {
        return coeficientesObjetivo;
    }

    const vector<Restricao>& getRestricoes() const {
        return restricoes;
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

class FormaPadrao {
private:
    vector<vector<double>> matriz;

    int qtdFolgasExcessos;
    int qtdArtificiais;
    int qtdColunas;

    void preencherMatriz(const ProblemaPL& problema) {

        int colunaFolgaExcesso = problema.getQtdVariaveis();

        int colunaArtificial = problema.getQtdVariaveis()
            + qtdFolgasExcessos;

        for (int i = 0; i < problema.getQtdRestricoes(); i++) {

            const Restricao& restricao = problema.getRestricoes()[i];

            for (int j = 0; j < problema.getQtdVariaveis(); j++) {

                matriz[i][j] = restricao.getCoeficientes()[j];
            }

            if (restricao.getOperador() == "<=") {

                matriz[i][colunaFolgaExcesso] = 1.0;
                colunaFolgaExcesso++;
            }

            else if (restricao.getOperador() == ">=") {

                matriz[i][colunaFolgaExcesso] = -1.0;
                matriz[i][colunaArtificial] = 1.0;
                colunaFolgaExcesso++;
                colunaArtificial++;
            }

            else if (restricao.getOperador() == "=") {

                matriz[i][colunaArtificial] = 1.0;
                colunaArtificial++;
            }
            
            matriz[i][qtdColunas - 1] =
                restricao.getLadoDireito();
        }
    }

public:
    FormaPadrao(const ProblemaPL& problema) {

        qtdFolgasExcessos = 0;
        qtdArtificiais = 0;

        // Conta quantas colunas auxiliares serão necessárias
        for (const Restricao& restricao :
             problema.getRestricoes()) {

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

        // x + folgas/excessos + artificiais + RHS
        qtdColunas =
            problema.getQtdVariaveis()
            + qtdFolgasExcessos
            + qtdArtificiais
            + 1;

        // Já cria a matriz no tamanho correto e preenchida com 0
        matriz.resize(
            problema.getQtdRestricoes(),
            vector<double>(qtdColunas, 0.0)
        );

        preencherMatriz(problema);
    }

    void imprimir() const {

        cout << "\nForma padrao:\n";

        for (const vector<double>& linha : matriz) {

            for (double valor : linha) {
                cout << valor << "\t";
            }

            cout << '\n';
        }
    }
};


// =========================
// Main
// =========================
int main() {

    ProblemaPL problema;

    problema.lerEntrada();
    problema.corrigirLadoDireitoNegativo();

    problema.imprimir();

    FormaPadrao formaPadrao(problema);

    formaPadrao.imprimir();

    return 0;
}