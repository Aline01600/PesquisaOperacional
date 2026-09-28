# Bateria de testes — Trabalho 1 (Simplex Duas Fases)

**Pesquisa Operacional — 2026/2 — Prof. Alberto Alexandre Assis Miranda**

Esta pasta contém **107 casos de teste** com gabarito, um script que confere
automaticamente a saída do seu programa e um gerador para os casos de grande porte.
A bateria é material de **estudo e autoavaliação**: passar em tudo aqui não garante
nota máxima (a correção usa também casos não divulgados), mas falhar aqui é
garantia de perder pontos.

---

## Conteúdo

```
casos/            entradas .txt (as do grupo G maiores vêm do gerador)
esperado/         gabarito de cada caso, no formato da Seção 5 do enunciado
verificar.py      confere a saída do seu programa contra o gabarito
gerador_grandes.py  reconstrói os casos g08..g13 (grandes demais para distribuir)
MANIFESTO.md      descrição caso a caso, matriz de cobertura e política de comparação
README.md         este arquivo
```

## Uso rápido

```bash
# 1. compile o seu programa
gcc -std=c11 -O2 -Wall -Wextra -o simplex simplex.c -lm      # ou g++ ...

# 2. rode um caso isolado, na mão
./simplex < casos/b01.txt
./simplex -t < casos/b01.txt        # com o passo a passo

# 3. rode a bateria inteira
python3 verificar.py ./simplex

# 4. rode só um grupo, enquanto está desenvolvendo
python3 verificar.py ./simplex --grupo A
python3 verificar.py ./simplex --caso f42 --verboso
```

Saída típica:

```
a01   ok      n=1    m=1     0.00s
...
f45   FALHA   n=2    m=2     0.00s  STATUS: esperado INVIAVEL, obtido OTIMA
--------------------------------------------------------------
  grupo A: 11/11 ok
  ...
```

Opções úteis do `verificar.py`:

| opção | efeito |
|---|---|
| `--grupo A B` | roda apenas os grupos indicados |
| `--caso f42 e15` | roda apenas os casos indicados |
| `--ate 200` | pula casos com `n + m` maior que o valor (útil no começo) |
| `--verboso` | imprime o gabarito e a sua saída lado a lado nas falhas |
| `--parar` | para na primeira falha |
| `--estrito` | transforma avisos (inclusive `ITERACOES`) em falhas |
| `--timeout 120` | tempo máximo por caso, em segundos (padrão 90) |

## Ordem recomendada de trabalho

A bateria foi montada em **complexidade crescente**, e essa ordem é também a ordem
sugerida para escrever o programa:

| grupo | o que exercita | quando atacar |
|---|---|---|
| **A** (11) | só `<=`: leitura, tableau, pivô, Fase 2 | primeiro programa que roda |
| **B** (7) | uma restrição `>=`: primeira artificial, Fase 1 | depois que A passa inteiro |
| **C** (6) | várias `>=` ao mesmo tempo | — |
| **D** (7) | igualdades, inclusive restrição redundante | fecha a padronização |
| **E** (22) | cada caso especial isolado no menor exemplo possível | ao implementar as detecções |
| **F** (41) | todas as combinações tipo de restrição × propriedade | revisão final |
| **G** (13) | escala, de `n=5` até `n=1600` | por último, para desempenho |

Um caso do grupo E ou F que falha enquanto A ainda tem falhas quase nunca é o
problema real — conserte de trás para frente.

## Casos grandes (grupo G)

Os arquivos `g08` a `g13` juntos passam de 20 MB, então não são distribuídos prontos.
Gere-os localmente (o resultado é idêntico em qualquer máquina, o gerador é determinístico):

```bash
python3 gerador_grandes.py            # gera todos os que faltam
python3 gerador_grandes.py g13        # gera apenas um
python3 gerador_grandes.py --lista    # mostra os tamanhos
```

O maior deles, `g13` (`n=1600`, `m=1920`), leva cerca de **30 segundos** na implementação
de referência. Ele existe para revelar o que os casos pequenos escondem: cópias
desnecessárias do tableau dentro do laço, vazamento de memória, limite de iterações mal
dimensionado e acúmulo de erro numérico. Não se assuste se ele levar 1 a 2 minutos no seu
programa; se levar 10, há algo estruturalmente errado no laço de pivoteamento.

Vale a pena abrir o `gerador_grandes.py` e ler o comentário do início: ele explica como
essas instâncias são construídas **já sabendo a resposta**, usando dualidade — assunto da
próxima unidade da disciplina.

## Como o gabarito foi produzido

Todos os arquivos de `esperado/` foram gerados por uma implementação de referência do
professor e conferidos, caso a caso, por uma **segunda implementação independente**
(linguagem diferente, escrita separadamente): as duas produzem saída idêntica em todos os
107 casos, incluindo a contagem de iterações. Nos casos do grupo G há ainda uma terceira
conferência: o valor ótimo é conhecido por construção matemática, sem rodar Simplex nenhum.

Se você acredita ter encontrado um erro no gabarito, poste no mural do Classroom com o
caso e o seu raciocínio — ganha ponto de participação se estiver certo.

## Aviso

Os casos aqui são **públicos e conhecidos**. Um programa que acerte estes 107 casos por
tratamento especial (comparar a entrada e imprimir a resposta decorada) será tratado como
fraude, e a correção final usa casos não divulgados justamente para detectar isso.
