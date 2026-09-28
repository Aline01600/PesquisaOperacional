#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Verificador da bateria de testes do Trabalho 1 (Simplex Duas Fases).

    python3 verificar.py ./simplex                  # roda a bateria toda
    python3 verificar.py ./simplex --grupo A B      # so os grupos indicados
    python3 verificar.py ./simplex --caso f42 e15   # so os casos indicados
    python3 verificar.py ./simplex --ate 200        # so casos com n+m <= 200
    python3 verificar.py ./simplex --estrito        # avisos viram falhas
    python3 verificar.py ./simplex --timeout 120    # segundos por caso (padrao 90)
    python3 verificar.py ./simplex -v               # mostra o diff de cada falha

O que e comparado (veja tambem o MANIFESTO.md):

  * STATUS ................ sempre, exato.
  * Z ..................... sempre, tolerancia 1e-4 (relativa para |Z| grande).
  * viabilidade de X ...... sempre; o verificador substitui o X do seu programa
                            nas restricoes originais e confere c.X = Z.
  * X, MULTIPLAS, DEGENERADA, ALTERNATIVA ... comparados com o gabarito nos
                            grupos A a F; no grupo G viram apenas AVISO, porque
                            instancias grandes podem ter caminhos de pivo
                            numericamente diferentes.
  * ITERACOES ............. AVISO por padrao (use --estrito para exigir).

AVISO nao reprova o caso, mas indica que algo saiu diferente do esperado -
vale investigar antes de entregar.
"""
import argparse, os, subprocess, sys, time

AQUI = os.path.dirname(os.path.abspath(__file__))
DIR_CASOS = os.path.join(AQUI, 'casos')
DIR_ESP = os.path.join(AQUI, 'esperado')
TOL = 1e-4
TOL_VIAB = 1e-5

VERDE, VERM, AMAR, CINZA, FIM = '\033[32m', '\033[31m', '\033[33m', '\033[90m', '\033[0m'
if not sys.stdout.isatty():
    VERDE = VERM = AMAR = CINZA = FIM = ''


def ler_modelo(caminho):
    tok = open(caminho).read().split()
    p = 0
    n = int(tok[p]); p += 1
    m = int(tok[p]); p += 1
    sentido = tok[p].upper(); p += 1
    c = [float(tok[p + j]) for j in range(n)]; p += n
    linhas = []
    for _ in range(m):
        a = [float(tok[p + j]) for j in range(n)]; p += n
        op = tok[p]; p += 1
        b = float(tok[p]); p += 1
        linhas.append((a, op, b))
    return n, m, sentido, c, linhas


def parse_saida(txt):
    d, ordem = {}, []
    for ln in txt.strip().split('\n'):
        ln = ln.strip()
        if not ln:
            continue
        k, sep, v = ln.partition(':')
        if not sep:
            return None, 'linha fora do formato da Secao 5: %r' % ln[:60]
        k = k.strip()
        if k not in ('STATUS', 'Z', 'X', 'MULTIPLAS', 'DEGENERADA',
                     'ITERACOES', 'ALTERNATIVA'):
            return None, 'chave desconhecida em stdout: %r' % k
        d[k] = v.strip(); ordem.append(k)
    if 'STATUS' not in d:
        return None, 'saida sem a linha STATUS'
    d['_ordem'] = ordem
    return d, None


def numeros(s):
    try:
        return [float(v) for v in s.split()]
    except ValueError:
        return None


def perto(a, b, tol=TOL):
    return abs(a - b) <= tol * max(1.0, abs(b))


def checa_viabilidade(modelo, x):
    n, m, sentido, c, linhas = modelo
    if len(x) != n:
        return 'X tem %d valores, esperado %d' % (len(x), n)
    for j, v in enumerate(x):
        if v < -TOL_VIAB:
            return 'x%d = %g e negativo' % (j + 1, v)
    for i, (a, op, b) in enumerate(linhas):
        lhs = sum(a[j] * x[j] for j in range(n))
        tol = TOL_VIAB * max(1.0, abs(b), abs(lhs))
        if op == '<=' and lhs > b + tol:
            return 'restricao %d violada: %g > %g' % (i + 1, lhs, b)
        if op == '>=' and lhs < b - tol:
            return 'restricao %d violada: %g < %g' % (i + 1, lhs, b)
        if op == '=' and abs(lhs - b) > tol:
            return 'restricao %d violada: %g != %g' % (i + 1, lhs, b)
    return None


def compara(cid, grupo, modelo, esp, got, estrito):
    """retorna (falhas, avisos)"""
    F, A = [], []
    if got['STATUS'] != esp['STATUS']:
        return ['STATUS: esperado %s, obtido %s' % (esp['STATUS'], got['STATUS'])], []
    if esp['STATUS'] != 'OTIMA':
        extras = [k for k in got if k not in ('STATUS', '_ordem')]
        if extras:
            A.append('linhas extras para STATUS %s: %s' % (esp['STATUS'], ', '.join(extras)))
        return F, A

    for k in ('Z', 'X', 'MULTIPLAS', 'DEGENERADA', 'ITERACOES'):
        if k not in got:
            F.append('falta a linha %s' % k)
    if F:
        return F, A

    ze, zg = float(esp['Z']), None
    try:
        zg = float(got['Z'])
    except ValueError:
        return ['Z nao e um numero: %r' % got['Z']], A
    if not perto(zg, ze):
        F.append('Z: esperado %.6f, obtido %.6f' % (ze, zg))

    xg = numeros(got['X'])
    if xg is None:
        F.append('X nao e uma lista de numeros')
    else:
        msg = checa_viabilidade(modelo, xg)
        if msg:
            F.append('X inviavel (%s)' % msg)
        else:
            n, m, sentido, c, linhas = modelo
            cx = sum(c[j] * xg[j] for j in range(n))
            if not perto(cx, zg):
                F.append('c.X = %.6f nao confere com o Z impresso (%.6f)' % (cx, zg))
        xe = numeros(esp['X'])
        if xe is not None and len(xe) == len(xg):
            dif = max(abs(a - b) for a, b in zip(xg, xe)) if xg else 0.0
            if dif > 1e-3:
                (F if (grupo != 'G' and estrito) else A).append(
                    'X difere do gabarito (maior diferenca %.4g) - outro vertice otimo?' % dif)

    for k in ('MULTIPLAS', 'DEGENERADA'):
        if got[k] != esp[k]:
            (F if grupo != 'G' else A).append(
                '%s: esperado %s, obtido %s' % (k, esp[k], got[k]))

    if got['ITERACOES'] != esp['ITERACOES']:
        (F if estrito and grupo != 'G' else A).append(
            'ITERACOES: esperado "%s", obtido "%s"' % (esp['ITERACOES'], got['ITERACOES']))

    tem_e, tem_g = 'ALTERNATIVA' in esp, 'ALTERNATIVA' in got
    if tem_e != tem_g:
        F.append('linha ALTERNATIVA %s' % ('faltando' if tem_e else 'a mais'))
    elif tem_e:
        ae, ag = esp['ALTERNATIVA'].strip(), got['ALTERNATIVA'].strip()
        if (ae == 'RAIO') != (ag == 'RAIO'):
            F.append('ALTERNATIVA: esperado %r, obtido %r' % (ae, ag))
        elif ae != 'RAIO':
            xa = numeros(ag)
            if xa is None:
                F.append('ALTERNATIVA nao e uma lista de numeros')
            else:
                msg = checa_viabilidade(modelo, xa)
                n, m, sentido, c, linhas = modelo
                if msg:
                    F.append('ALTERNATIVA inviavel (%s)' % msg)
                elif not perto(sum(c[j] * xa[j] for j in range(n)), zg):
                    F.append('ALTERNATIVA nao tem o mesmo valor de Z')
                else:
                    xe = numeros(ae)
                    if xe and max(abs(a - b) for a, b in zip(xa, xe)) > 1e-3:
                        (F if (grupo != 'G' and estrito) else A).append(
                            'ALTERNATIVA difere do gabarito (mas e otima e viavel)')
    return F, A


def main():
    ap = argparse.ArgumentParser(description='Verificador da bateria do Trabalho 1')
    ap.add_argument('programa', help='caminho do seu executavel (ex.: ./simplex)')
    ap.add_argument('--grupo', nargs='+', default=None)
    ap.add_argument('--caso', nargs='+', default=None)
    ap.add_argument('--ate', type=int, default=None, help='so casos com n+m <= valor')
    ap.add_argument('--timeout', type=float, default=90.0)
    ap.add_argument('--estrito', action='store_true')
    ap.add_argument('-v', '--verboso', action='store_true')
    ap.add_argument('--parar', action='store_true', help='parar na primeira falha')
    args = ap.parse_args()

    if not os.path.isdir(DIR_CASOS):
        print('nao encontrei a pasta casos/ ao lado de verificar.py'); sys.exit(1)

    ids = sorted(f[:-4] for f in os.listdir(DIR_ESP) if f.endswith('.out'))
    if args.grupo:
        gs = tuple(g.upper() for g in args.grupo)
        ids = [i for i in ids if i[0].upper() in gs]
    if args.caso:
        alvo = set(c.lower() for c in args.caso)
        ids = [i for i in ids if i.lower() in alvo]

    tot = ok = falhou = avisou = pulou = 0
    t_ini = time.time()
    por_grupo = {}
    for cid in ids:
        grupo = cid[0].upper()
        cam = os.path.join(DIR_CASOS, cid + '.txt')
        if not os.path.exists(cam):
            print('%s%-5s PULADO%s  arquivo casos/%s.txt ausente '
                  '(gere com: python3 gerador_grandes.py %s)' % (CINZA, cid, FIM, cid, cid))
            pulou += 1
            continue
        modelo = ler_modelo(cam)
        if args.ate is not None and modelo[0] + modelo[1] > args.ate:
            pulou += 1
            continue
        esp, err = parse_saida(open(os.path.join(DIR_ESP, cid + '.out')).read())
        tot += 1
        t0 = time.time()
        try:
            with open(cam) as fh:
                r = subprocess.run([args.programa], stdin=fh, capture_output=True,
                                   text=True, timeout=args.timeout)
        except subprocess.TimeoutExpired:
            print('%s%-5s FALHA %s  estourou o tempo limite de %.0fs'
                  % (VERM, cid, FIM, args.timeout))
            falhou += 1
            por_grupo.setdefault(grupo, [0, 0])[1] += 1
            if args.parar:
                break
            continue
        except OSError as e:
            print('nao consegui executar %r: %s' % (args.programa, e)); sys.exit(1)
        dt = time.time() - t0
        if r.returncode != 0:
            F, A = ['o programa terminou com codigo %d' % r.returncode], []
            if r.stderr.strip():
                F.append('stderr: ' + r.stderr.strip().split('\n')[0][:80])
        else:
            got, err2 = parse_saida(r.stdout)
            if got is None:
                F, A = [err2], []
            else:
                F, A = compara(cid, grupo, modelo, esp, got, args.estrito)
        g = por_grupo.setdefault(grupo, [0, 0])
        g[0] += 1
        if F:
            falhou += 1
            g[1] += 1
            print('%s%-5s FALHA %s  n=%-4d m=%-4d %5.2fs  %s'
                  % (VERM, cid, FIM, modelo[0], modelo[1], dt, F[0]))
            for extra in F[1:]:
                print('      %s' % extra)
            if args.verboso:
                print(CINZA + '      --- esperado ---' + FIM)
                for ln in open(os.path.join(DIR_ESP, cid + '.out')).read().strip().split('\n'):
                    print('      ' + ln)
                print(CINZA + '      --- obtido ---' + FIM)
                for ln in (r.stdout.strip().split('\n') if r.stdout.strip() else ['(vazio)']):
                    print('      ' + ln)
            if args.parar:
                break
        else:
            ok += 1
            if A:
                avisou += 1
                print('%s%-5s AVISO %s  n=%-4d m=%-4d %5.2fs  %s'
                      % (AMAR, cid, FIM, modelo[0], modelo[1], dt, '; '.join(A)))
            else:
                print('%s%-5s ok    %s  n=%-4d m=%-4d %5.2fs'
                      % (VERDE, cid, FIM, modelo[0], modelo[1], dt))

    print('\n' + '-' * 62)
    for g in sorted(por_grupo):
        n_, f_ = por_grupo[g]
        print('  grupo %s: %d/%d ok' % (g, n_ - f_, n_))
    print('-' * 62)
    print('%d casos executados | %s%d ok%s | %s%d com aviso%s | %s%d falhas%s | %d pulados | %.1fs'
          % (tot, VERDE, ok, FIM, AMAR, avisou, FIM, VERM, falhou, FIM, pulou, time.time() - t_ini))
    sys.exit(1 if falhou else 0)


if __name__ == '__main__':
    main()
