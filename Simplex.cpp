#include "Simplex.h"

const double EPS = 1e-9;

Simplex::Simplex(Tableau& tableau) : tableau(tableau) {
}

int Simplex::entraNaBase() {

    vector<vector<double>>& matriz = tableau.getMatriz();

    int linhaObjetivo = tableau.getLinhaObjetivo();
    int qtdColunas = tableau.getQtdColunas();

    int colunaEntrada = -1;
    double menorValor = -EPS;

    for (int j = 0; j < qtdColunas - 1; j++) {

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

    for (int j = 0; j < qtdColunas; j++) {
        matriz[linhaPivo][j] /= elementoPivo; 
    }

    for (int i = 0; i < qtdLinhas; i++) {

        if (i != linhaPivo) {

            double multiplicador = matriz[i][colunaPivo];

            for (int j = 0; j < qtdColunas; j++) {
                matriz[i][j] -= multiplicador * matriz[linhaPivo][j];
            }
        }
    }

    base[linhaPivo] = colunaPivo;
}

bool Simplex::resolverFase2() {

    while (true) {

        int colunaEntrada = entraNaBase();

        if (colunaEntrada == -1) {
            return true;
        }

        int linhaSaida = saiDaBase(colunaEntrada);

        if (linhaSaida == -1) {
            return false;
        }

        pivotear(linhaSaida, colunaEntrada);
        tableau.imprimir();
    }
}