from copy import deepcopy
import numpy as np
from polynom import Polynom, ext_Euclid, invert_mine

p = 3
q = 32
f_values = (-1, 1, 1, 0, -1, 0, 1, 0, 0, 1, -1)
g_values = (-1, 0, 1, 1, 0, 1, 0, 0, -1, 0, -1)
#           0   1  2  3  4  5  6  7   8  9  10
m_values = (-1, 0, 0, 1, -1, 0, 0, 0, -1, 1, 1)
r_values = (-1, 0, 1, 1, 1, -1, 0, -1, 0, 0, 0)


def Euclid_test():
    f1 = Polynom(f_values, p)
    f2 = Polynom((1, 1), p)

    d, a, b = ext_Euclid(f1, f2)

    print((a * f1 + b * f2).coefs)
    print(d.coefs)


def test_NTRU():
    Polynom.CUR_MOD = p
    f = Polynom(f_values)
    fp = invert_mine(f)

    Polynom.CUR_MOD = q
    f = Polynom(f_values)
    fq = invert_mine(f)

    g = Polynom(g_values)

    h = Polynom(p * fq.coefs) * g  # PUBLIC
    h.apply_mod()

    print("fp : ", fp)
    print("fq : ", fq)
    print("h  : ", h)

    print("\nEncryption")

    m = Polynom(m_values)
    r = Polynom(r_values)

    print("m  : ", m)
    print("r  : ", r)

    e = r * h + m  # MESSAGE
    e.apply_mod()
    print(e)

    print("\nDecryption")

    f = Polynom(f_values)
    a = f * e
    a.apply_halfmod()
    print("a : ", a)

    Polynom.CUR_MOD = p
    b = a
    b.apply_halfmod()

    print("b  : ", b)

    c = fp * f * m
    c.apply_halfmod()

    print("c : ", c)


def get_invertable(low, up):
    found = False
    res = None
    while not found:
        try:
            # if True:
            res = np.random.randint(low, up + 1, size=Polynom.N)

            Polynom.CUR_MOD = p
            inv1 = invert_mine(Polynom(res))
            Polynom.CUR_MOD = q
            inv2 = invert_mine(Polynom(res))
            found = True
        except:
            print("Not good, not good at all")
            found = False
    return res

def get_random(low, up):
    return np.random.randint(low, up + 1, size=Polynom.N)

def init_global_vars():
    global p, q, f_values, m_values, r_values, g_values
    p = 71
    q = 128
    Polynom.N = 31

    f_values = get_invertable(-1, 1)
    m_values = get_random(-1, 1)

    r_values = get_random(0, p * q)
    g_values = get_random(0, p * q)


init_global_vars()
test_NTRU()
