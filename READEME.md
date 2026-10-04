# Trabalho 1 - Simplex de Duas Fases
Implementacao do metodo Simplex de Duas Fases em C++17.
## Compilacao
make
Ou:
g++ -std=c++17 -O2 -Wall -Wextra main.cpp Restricao.cpp ProblemaPL.cpp FormaPadrao.cpp Tableau.cpp Simplex.cpp -o simplex
## Execucao
Modo normal:
./simplex < entrada.txt
Modo passo a passo:
./simplex -t < entrada.txt
## Organizacao
O programa foi dividido nas classes Restricao, ProblemaPL, FormaPadrao, Tableau e Simplex.
O tableau utiliza `vector<vector<double>>`, permitindo alocacao dinamica conforme a quantidade de variaveis e restricoes.
As restricoes sao tratadas da seguinte forma:
`<=` adiciona variavel de folga  
`>=` adiciona variavel de excesso e artificial  
`=` adiciona variavel artificial
Na Fase 1, o programa busca uma solucao basica viavel e elimina as variaveis artificiais.
Na Fase 2, a funcao objetivo original e restaurada e o Simplex continua ate encontrar a solucao otima ou identificar um problema ilimitado.
O programa tambem identifica problemas inviaveis, multiplas solucoes, degenerescencia e solucoes alternativas.
## Bateria de testes
107 casos executados  
107 ok  
0 com aviso  
0 falhas  
0 pulados
## Duas Fases x Big-M
O metodo das Duas Fases e preferivel em uma implementacao computacional porque nao depende da escolha de um valor muito grande para M.
No Big-M, valores muito grandes podem causar problemas numericos, enquanto valores pequenos podem nao penalizar corretamente as variaveis artificiais.
Nas Duas Fases, primeiro e encontrada uma solucao viavel e depois a funcao objetivo original e otimizada.
## Degenerescencia e multiplas solucoes
Multiplas solucoes sao identificadas quando existe uma variavel nao basica com custo reduzido aproximadamente igual a zero.
Em casos degenerados, um pivoteamento nessa coluna pode apenas gerar outra base que representa o mesmo ponto, e nao necessariamente um vertice otimo diferente.