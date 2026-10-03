#include "ProblemaPL.h"
#include "FormaPadrao.h"
#include "Tableau.h"

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

    tableau.imprimir();

    return 0;
}