#ifndef SIMPLEX_H
#define SIMPLEX_H

#include "Tableau.h"

class Simplex {
private:
    Tableau& tableau;

    int entraNaBase();
    int saiDaBase(int colunaEntrada);
    void pivotear(int linhaPivo, int colunaPivo);

public:
    Simplex(Tableau& tableau);

    bool resolverFase2();
};

#endif