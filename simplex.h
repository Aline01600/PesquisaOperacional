#ifndef SIMPLEX_H
#define SIMPLEX_H

#include <vector>
#include "Tableau.h"

using namespace std;

class Simplex {
private:
    Tableau& tableau;

    bool modoTrace;

    int iteracoesFase1;
    int iteracoesFase2;

    int entraNaBase(int limiteColunas);
    int saiDaBase(int colunaEntrada);
    void pivotear(int linhaPivo, int colunaPivo);
    int encontrarColunaAlternativa();

public:
    Simplex(Tableau& tableau, bool modoTrace = false);

    bool resolverFase1(int inicioArtificiais);
    void transicaoFase1Fase2(int inicioArtificiais, int qtdArtificiais);
    bool resolverFase2();

    bool temMultiplasSolucoes();
    bool gerarSolucaoAlternativa();
    bool solucaoDegenerada();

    vector<double> getSolucao(int qtdVariaveis);
    double getValorZ();

    int getIteracoesFase1();
    int getIteracoesFase2();
};

#endif