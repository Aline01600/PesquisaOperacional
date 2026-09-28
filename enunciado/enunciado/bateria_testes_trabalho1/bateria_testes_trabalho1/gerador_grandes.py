#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Gerador dos casos de grande porte (grupo G) da bateria do Trabalho 1.

Os arquivos grandes NAO sao distribuidos prontos (alguns passam de 5 MB).
Este script os reconstroi de forma deterministica: rodando-o em qualquer
maquina, com qualquer versao de Python 3, voce obtem exatamente os mesmos
arquivos usados para gerar os gabaritos em 'esperado/'.

    python3 gerador_grandes.py                # gera todos os casos faltantes
    python3 gerador_grandes.py g10 g11        # gera apenas os casos indicados
    python3 gerador_grandes.py --lista        # lista os casos e seus tamanhos

------------------------------------------------------------------------
COMO ESTES CASOS SAO CONSTRUIDOS (vale a pena ler - e teoria da disciplina)
------------------------------------------------------------------------
Nao adianta sortear uma matriz A, um vetor b e um vetor c ao acaso: nesse
caso ninguem sabe qual e a resposta certa. Aqui fazemos o caminho inverso,
usando DUALIDADE (assunto da proxima unidade):

1. sorteia-se A >= 0 e um ponto x* > 0 que sera o otimo desejado;
2. escolhe-se um conjunto ATIVO com exatamente n restricoes (todas as de
   igualdade mais algumas do tipo <=). Para essas, faz-se b_i = A_i x*
   (ficam justas); para as demais sobra folga estritamente positiva;
3. sorteiam-se multiplicadores y_i > 0 nas <= ativas, y_i de sinal
   qualquer nas igualdades e y_i = 0 nas demais, e define-se c = A^T y.

Para qualquer x viavel vale entao
        c x = y^T A x <= y^T b = c x*,
ou seja, x* e otimo e Z* = c x* e conhecido de antemao, sem rodar Simplex
nenhum. Como as n restricoes ativas sao linearmente independentes e todos
os y_i das ativas sao nao nulos, o otimo e unico e nao degenerado.
"""
import sys, os

# id, n, m, qtd de restricoes >=, qtd de restricoes =, semente
SPECS = [
    ('g01',    5,     6,   0,   0,  1001),
    ('g02',   10,    12,   2,   1,  1002),
    ('g03',   20,    25,   4,   2,  1003),
    ('g04',   40,    50,   8,   3,  1004),
    ('g05',   80,   100,  15,   5,  1005),
    ('g06',  150,   180,  25,   8,  1006),
    ('g07',  250,   300,  40,  10,  1007),
    ('g08',  400,   480,   0,   0,  1008),
    ('g09',  600,   720,  60,  15,  1009),
    ('g10',  800,   960,   0,   0,  1010),
    ('g11', 1000,  1200, 100,  20,  1011),
    ('g12', 1250,  1500,   0,   0,  1012),
    ('g13', 1600,  1920,   0,   0,  1013),
]
# casos ja distribuidos prontos dentro de casos/
JA_INCLUSOS = {'g01', 'g02', 'g03', 'g04', 'g05', 'g06', 'g07'}


class Rng(object):
    """LCG classico (mesmo dos livros de C). Fixado aqui para que o
    resultado nao dependa da versao do Python nem do sistema."""
    def __init__(self, seed):
        self.s = seed & 0x7FFFFFFF

    def next(self):
        self.s = (1103515245 * self.s + 12345) & 0x7FFFFFFF
        return self.s >> 16          # bits altos: melhor qualidade

    def entre(self, a, b):           # inteiro em [a, b]
        return a + self.next() % (b - a + 1)

    def embaralhar(self, v):
        for i in range(len(v) - 1, 0, -1):
            j = self.next() % (i + 1)
            v[i], v[j] = v[j], v[i]
        return v


def gerar(spec):
    cid, n, m, m_ge, m_eq, seed = spec
    m_le = m - m_ge - m_eq
    if m_le < n - m_eq:
        raise ValueError('%s: restricoes <= insuficientes' % cid)
    r = Rng(seed)

    A = [[r.entre(1, 9) for _ in range(n)] for _ in range(m)]
    xs = [r.entre(1, 9) for _ in range(n)]

    # tipos de restricao espalhados pelas linhas
    tipos = ['<='] * m_le + ['>='] * m_ge + ['='] * m_eq
    r.embaralhar(tipos)

    lin_le = [i for i in range(m) if tipos[i] == '<=']
    r.embaralhar(lin_le)
    ativas_le = set(lin_le[:n - m_eq])          # <= que ficarao justas

    y = [0] * m
    for i in range(m):
        if tipos[i] == '=':
            y[i] = r.entre(1, 5) * (1 if r.next() % 2 else -1)
        elif i in ativas_le:
            y[i] = r.entre(1, 5)

    b, Ax = [], []
    for i in range(m):
        v = sum(A[i][j] * xs[j] for j in range(n))
        Ax.append(v)
        if tipos[i] == '=' or i in ativas_le:
            b.append(v)                                    # justa
        elif tipos[i] == '<=':
            b.append(v + r.entre(1, 20))                   # folga positiva
        else:                                              # >= inativa
            b.append(v - r.entre(1, min(20, max(1, v - 1))))
    c = [sum(A[i][j] * y[i] for i in range(m) if y[i]) for j in range(n)]

    linhas = ['%d %d' % (n, m), 'MAX', ' '.join(map(str, c))]
    for i in range(m):
        linhas.append(' '.join(map(str, A[i])) + ' ' + tipos[i] + ' ' + str(b[i]))
    zstar = sum(c[j] * xs[j] for j in range(n))
    return '\n'.join(linhas) + '\n', zstar, xs


def main():
    args = [a for a in sys.argv[1:]]
    if '--lista' in args:
        print('%-5s %6s %6s %6s %6s' % ('caso', 'n', 'm', '>=', '='))
        for s in SPECS:
            print('%-5s %6d %6d %6d %6d%s' %
                  (s[0], s[1], s[2], s[3], s[4],
                   '   (ja incluso)' if s[0] in JA_INCLUSOS else ''))
        return
    alvo = [a for a in args if not a.startswith('-')]
    destino = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'casos')
    if not os.path.isdir(destino):
        destino = 'casos'
        os.makedirs(destino, exist_ok=True)
    for s in SPECS:
        if alvo and s[0] not in alvo:
            continue
        if not alvo and s[0] in JA_INCLUSOS:
            continue
        cam = os.path.join(destino, s[0] + '.txt')
        txt, z, _ = gerar(s)
        with open(cam, 'w') as fh:
            fh.write(txt)
        print('%s: n=%d m=%d  Z* = %d  -> %s (%.1f MB)' %
              (s[0], s[1], s[2], z, cam, len(txt) / 1e6))


if __name__ == '__main__':
    main()
