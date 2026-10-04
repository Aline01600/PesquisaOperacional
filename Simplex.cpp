#include "Simplex.h"
#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

const double EPS = 1e-9;
const double EPS_CLASSIFICACAO = 1e-7;

Simplex::Simplex(Tableau& tableau, bool modoTrace) : tableau(tableau) {
    this->modoTrace = modoTrace;
    iteracoesFase1 = 0;
    iteracoesFase2 = 0;
}

int Simplex::entraNaBase(int limiteColunas) {

    vector<vector<double>>& matriz = tableau.getMatriz();

    int linhaObjetivo = tableau.getLinhaObjetivo();

    int colunaEntrada = -1;
    double menorValor = -EPS;

    for (int j = 0; j < limiteColunas; j++) {

        if (matriz[linhaObjetivo][j] < menorValor) {

            menorValor = matriz[linhaObjetivo][j];
            colunaEntrada = j;
        }
    }

    return colunaEntrada;
}

int Simplex::saiDaBase(int colunaEntrada) {

    vector<vector<double>>& matriz = tableau.getMatriz();

    int qtdLinhas = tableau.getQtdLinhas();
    int qtdColunas = tableau.getQtdColunas();

    int linhaSaida = -1;
    double menorRazao = 0.0;
    bool encontrouRazao = false;

    for (int i = 0; i < qtdLinhas - 1; i++) {

        double coeficiente = matriz[i][colunaEntrada];

        if (coeficiente > EPS) {

            double rhs = matriz[i][qtdColunas - 1];
            double razao = rhs / coeficiente;

            if (!encontrouRazao || razao < menorRazao - EPS) {

                menorRazao = razao;
                linhaSaida = i;
                encontrouRazao = true;
            }
        }
    }

    return linhaSaida;
}

void Simplex::pivotear(int linhaPivo, int colunaPivo) {

    vector<vector<double>>& matriz = tableau.getMatriz();
    vector<int>& base = tableau.getBase();

    int qtdLinhas = tableau.getQtdLinhas();
    int qtdColunas = tableau.getQtdColunas();

    double elementoPivo = matriz[linhaPivo][colunaPivo];

    if (abs(elementoPivo - 1.0) > EPS) {

        for (int j = 0; j < qtdColunas; j++) {
            matriz[linhaPivo][j] /= elementoPivo;
        }
    }

    for (int i = 0; i < qtdLinhas; i++) {

        if (i != linhaPivo) {

            double multiplicador = matriz[i][colunaPivo];

            if (abs(multiplicador) > EPS) {

                for (int j = 0; j < qtdColunas; j++) {
                    matriz[i][j] -= multiplicador * matriz[linhaPivo][j];
                }
            }
        }
    }

    base[linhaPivo] = colunaPivo;
}

bool Simplex::resolverFase1(int inicioArtificiais) {

    while (true) {

        int colunaEntrada = entraNaBase(inicioArtificiais);

        if (colunaEntrada == -1) {
            break;
        }

        int linhaSaida = saiDaBase(colunaEntrada);

        if (linhaSaida == -1) {
            return false;
        }

        if (modoTrace) {

            vector<int>& base = tableau.getBase();
            vector<vector<double>>& matriz = tableau.getMatriz();

            cout << "Entra: " << tableau.getNomeColuna(colunaEntrada)
                 << " | Sai: " << tableau.getNomeColuna(base[linhaSaida])
                 << " | Pivo: linha " << linhaSaida + 1
                 << ", coluna " << tableau.getNomeColuna(colunaEntrada)
                 << " (valor " << fixed << setprecision(4)
                 << matriz[linhaSaida][colunaEntrada] << ")\n\n";
        }

        pivotear(linhaSaida, colunaEntrada);

        iteracoesFase1++;

        if (modoTrace) {

            cout << "--- Tableau apos iteracao "
                 << iteracoesFase1
                 << " (Fase 1) ---\n";

            tableau.imprimir();
        }
    }

    vector<vector<double>>& matriz = tableau.getMatriz();

    int linhaObjetivo = tableau.getLinhaObjetivo();
    int qtdColunas = tableau.getQtdColunas();

    double valorW = -matriz[linhaObjetivo][qtdColunas - 1];

    if (abs(valorW) < EPS_CLASSIFICACAO) {
        valorW = 0.0;
    }

    if (modoTrace) {

        cout << "Fase 1 concluida: w = "
             << fixed << setprecision(6)
             << valorW << "\n\n";
    }

    if (valorW > EPS_CLASSIFICACAO) {
        return false;
    }

    return true;
}

void Simplex::transicaoFase1Fase2(int inicioArtificiais, int qtdArtificiais) {

    vector<vector<double>>& matriz = tableau.getMatriz();
    vector<int>& base = tableau.getBase();

    int fimArtificiais = inicioArtificiais + qtdArtificiais;

    int i = 0;

    while (i < (int)base.size()) {

        int colunaBasica = base[i];

        bool artificialNaBase =
            colunaBasica >= inicioArtificiais &&
            colunaBasica < fimArtificiais;

        if (artificialNaBase) {

            int colunaPivo = -1;

            for (int j = 0; j < inicioArtificiais; j++) {

                if (abs(matriz[i][j]) > EPS) {
                    colunaPivo = j;
                    break;
                }
            }

            if (colunaPivo != -1) {

                pivotear(i, colunaPivo);
                i++;
            } else {

                tableau.removerLinha(i);
            }
            
        }else {
            i++;
        }
    }

    tableau.removerColunasArtificiais(inicioArtificiais, qtdArtificiais);
}

bool Simplex::resolverFase2() {

    while (true) {

        int limiteColunas = tableau.getQtdColunas() - 1;

        int colunaEntrada = entraNaBase(limiteColunas);

        if (colunaEntrada == -1) {
            return true;
        }

        int linhaSaida = saiDaBase(colunaEntrada);

        if (linhaSaida == -1) {
            return false;
        }

        if (modoTrace) {

            vector<int>& base = tableau.getBase();
            vector<vector<double>>& matriz = tableau.getMatriz();

            cout << "Entra: " << tableau.getNomeColuna(colunaEntrada)
                 << " | Sai: " << tableau.getNomeColuna(base[linhaSaida])
                 << " | Pivo: linha " << linhaSaida + 1
                 << ", coluna " << tableau.getNomeColuna(colunaEntrada)
                 << " (valor " << fixed << setprecision(4)
                 << matriz[linhaSaida][colunaEntrada] << ")\n\n";
        }

        pivotear(linhaSaida, colunaEntrada);

        iteracoesFase2++;

        if (modoTrace) {

            cout << "--- Tableau apos iteracao "
                 << iteracoesFase2
                 << " (Fase 2) ---\n";

            tableau.imprimir();
        }
    }
}

int Simplex::encontrarColunaAlternativa() {

    vector<vector<double>>& matriz = tableau.getMatriz();
    vector<int>& base = tableau.getBase();

    int linhaObjetivo = tableau.getLinhaObjetivo();
    int qtdColunas = tableau.getQtdColunas();

    for (int j = 0; j < qtdColunas - 1; j++) {

        bool estaNaBase = false;

        for (int i = 0; i < (int)base.size(); i++) {

            if (base[i] == j) {
                estaNaBase = true;
                break;
            }
        }

        if (!estaNaBase &&
            abs(matriz[linhaObjetivo][j]) < EPS_CLASSIFICACAO) {

            return j;
        }
    }

    return -1;
}

bool Simplex::temMultiplasSolucoes() {
    return encontrarColunaAlternativa() != -1;
}

bool Simplex::gerarSolucaoAlternativa() {

    int colunaEntrada = encontrarColunaAlternativa();

    if (colunaEntrada == -1) {
        return false;
    }

    int linhaSaida = saiDaBase(colunaEntrada);

    if (linhaSaida == -1) {
        return false;
    }

    pivotear(linhaSaida, colunaEntrada);

    return true;
}

bool Simplex::solucaoDegenerada() {

    vector<vector<double>>& matriz = tableau.getMatriz();

    int qtdLinhas = tableau.getQtdLinhas();
    int qtdColunas = tableau.getQtdColunas();

    for (int i = 0; i < qtdLinhas - 1; i++) {

        double valorBasica = matriz[i][qtdColunas - 1];

        if (abs(valorBasica) < EPS_CLASSIFICACAO) {
            return true;
        }
    }

    return false;
}

vector<double> Simplex::getSolucao(int qtdVariaveis) {

    vector<vector<double>>& matriz = tableau.getMatriz();
    vector<int>& base = tableau.getBase();

    int qtdColunas = tableau.getQtdColunas();

    vector<double> solucao(qtdVariaveis, 0.0);

    for (int i = 0; i < (int)base.size(); i++) {

        int colunaBasica = base[i];

        if (colunaBasica < qtdVariaveis) {
            solucao[colunaBasica] = matriz[i][qtdColunas - 1];
        }
    }

    return solucao;
}

double Simplex::getValorZ() {

    vector<vector<double>>& matriz = tableau.getMatriz();

    int linhaObjetivo = tableau.getLinhaObjetivo();
    int qtdColunas = tableau.getQtdColunas();

    return matriz[linhaObjetivo][qtdColunas - 1];
}

int Simplex::getIteracoesFase1() {
    return iteracoesFase1;
}

int Simplex::getIteracoesFase2() {
    return iteracoesFase2;
}