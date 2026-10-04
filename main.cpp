#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>
#include <string>

#include "ProblemaPL.h"
#include "FormaPadrao.h"
#include "Tableau.h"
#include "Simplex.h"

using namespace std;

double ajustarZero(double valor) {
    if (abs(valor) < 1e-9) {
        return 0.0;
    }

    return valor;
}

int main(int argc, char* argv[]) {

    bool modoTrace = false;

    for (int i = 1; i < argc; i++) {
        if (string(argv[i]) == "-t") {
            modoTrace = true;
        }
    }

    ProblemaPL problema;

    problema.lerEntrada();
    problema.corrigirLadoDireitoNegativo();

    FormaPadrao formaPadrao(problema);
    Tableau tableau(formaPadrao, problema);
    Simplex simplex(tableau, modoTrace);

    bool problemaViavel = true;
    bool encontrouOtimo = true;

    if (formaPadrao.getQtdArtificiais() > 0) {

        tableau.montarFase1(formaPadrao, problema);

        if (modoTrace) {
            cout << "\n---------------------- FASE 1 --------------------------\n";
            cout << "\nTableau inicial\n";
            tableau.imprimir();
        }

        int inicioArtificiais = problema.getQtdVariaveis() + formaPadrao.getQtdFolgasExcessos();

        problemaViavel = simplex.resolverFase1(inicioArtificiais);

        if (problemaViavel) {

            simplex.transicaoFase1Fase2(inicioArtificiais, formaPadrao.getQtdArtificiais());

            tableau.montarFase2(problema);

            if (modoTrace) {
                cout << "\n---------------------- FASE 2 --------------------------\n";
                cout << "\nTableau inicial\n";
                tableau.imprimir();
            }

            encontrouOtimo = simplex.resolverFase2();
        }
    }

    else {

        tableau.montarFase2(problema);

        if (modoTrace) {
            cout << "\n---------------------- FASE 2 --------------------------\n";
            cout << "\nTableau inicial\n";
            tableau.imprimir();
        }

        encontrouOtimo = simplex.resolverFase2();
    }

    if (!problemaViavel) {
        cout << "STATUS: INVIAVEL\n";
        return 0;
    }

    if (!encontrouOtimo) {
        cout << "STATUS: ILIMITADA\n";
        return 0;
    }

    vector<double> solucao = simplex.getSolucao(problema.getQtdVariaveis());
    double valorZ = simplex.getValorZ();

    if (problema.getTipoOtimizacao() == "MIN") {
        valorZ *= -1;
    }

    valorZ = ajustarZero(valorZ);

    bool multiplas = simplex.temMultiplasSolucoes();
    bool degenerada = simplex.solucaoDegenerada();

    cout << fixed << setprecision(6);

    cout << "STATUS: OTIMA\n";
    cout << "Z: " << valorZ << '\n';

    cout << "X:";

    for (int i = 0; i < (int)solucao.size(); i++) {
        cout << " " << ajustarZero(solucao[i]);
    }

    cout << '\n';

    cout << "MULTIPLAS: " << (multiplas ? "SIM" : "NAO") << '\n';
    cout << "DEGENERADA: " << (degenerada ? "SIM" : "NAO") << '\n';

    cout << "ITERACOES: "
         << simplex.getIteracoesFase1()
         << " "
         << simplex.getIteracoesFase2()
         << '\n';

    if (multiplas) {

        bool gerouAlternativa = simplex.gerarSolucaoAlternativa();

        if (gerouAlternativa) {

            vector<double> alternativa = simplex.getSolucao(problema.getQtdVariaveis());

            cout << "ALTERNATIVA:";

            for (int i = 0; i < (int)alternativa.size(); i++) {
                cout << " " << ajustarZero(alternativa[i]);
            }

            cout << '\n';
        }

        else {
            cout << "ALTERNATIVA: RAIO\n";
        }
    }

    return 0;
}