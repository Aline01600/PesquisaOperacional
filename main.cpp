#include <iostream>

#include "ProblemaPL.h"
#include "FormaPadrao.h"
#include "Tableau.h"
#include "Simplex.h"

using namespace std;

int main() {

    ProblemaPL problema;

    problema.lerEntrada();
    problema.corrigirLadoDireitoNegativo();

    FormaPadrao formaPadrao(problema);

    formaPadrao.imprimir();

    Tableau tableau(formaPadrao, problema);

    if (formaPadrao.getQtdArtificiais() > 0) {
        tableau.montarFase1(formaPadrao, problema);
    }
    else {
        tableau.montarFase2(problema);
    }

    cout << "\nTableau inicial:\n";
    tableau.imprimir();

    if (formaPadrao.getQtdArtificiais() == 0) {

        Simplex simplex(tableau);

        bool encontrouOtimo = simplex.resolverFase2();

        if (encontrouOtimo) {
            cout << "\nOtimo encontrado\n";
        }
        else {
            cout << "\nProblema ilimitado\n";
        }
    }

    return 0;
}