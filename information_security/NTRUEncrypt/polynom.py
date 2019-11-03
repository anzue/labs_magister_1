from copy import deepcopy

import numpy as np
from numpy.polynomial.tests.test_classes import Poly
import math
from sympy import GF, invert
import logging
import numpy as np
from sympy.abc import x
from sympy import ZZ, Poly

class Polynom:
    N = 11
    AUTO_POW_APPLY = True
    CUR_MOD = 0

    def __init__(self, coefs=(0,)):
        self.coefs = np.asarray(coefs)

    def apply_halfmod(self):
        for i in range(len(self.coefs)):
            self.coefs[i] = self.coefs[i] % Polynom.CUR_MOD
            if 2 * self.coefs[i] > Polynom.CUR_MOD:
                self.coefs[i] -= Polynom.CUR_MOD

    def apply_mod(self):
        for i in range(len(self.coefs)):
            self.coefs[i] = (self.coefs[i] % Polynom.CUR_MOD + Polynom.CUR_MOD) % Polynom.CUR_MOD

    def get_value_at(self, id):
        if 0 <= id < len(self.coefs):
            return self.coefs[id]
        return 0

    def normalize(self):
        pos = len(self.coefs) - 1
        while pos > 0 and self.coefs[pos] == 0:
            pos -= 1
        self.coefs = self.coefs[:pos + 1]

    def apply_power(self):
        for i in range(Polynom.N, len(self.coefs)):
            self.coefs[i % Polynom.N] += self.coefs[i]
            self.coefs[i] = 0

    def print(self):
        print("Len = " + str(len(self.coefs)) + " Coefs = " + str(
            self.coefs) + "\nAs polynom = ", end='')
        for i in range(len(self.coefs) - 1, -1, -1):
            if self.coefs[i] != 0:
                if self.coefs[i] > 0:
                    print("+", end='')
                print(str(self.coefs[i]) + "*x^" + str(i), end='')
        print("")

    def __add__(p1, p2):
        res_len = max(len(p1.coefs), len(p2.coefs))
        res = Polynom(np.zeros(res_len, dtype=int))
        for i in range(res_len):
            res.coefs[i] = (p1.get_value_at(i) + p2.get_value_at(i))
        res.normalize()
        return res

    def __mul__(p1, p2):
        res_coefs = np.zeros(len(p1.coefs) * len(p2.coefs), dtype=int)
        for i in range(len(p1.coefs)):
            for j in range(len(p2.coefs)):
                res_coefs[i + j] += p1.coefs[i] * p2.coefs[j]
        res = Polynom(res_coefs)

        if Polynom.AUTO_POW_APPLY:
            res.apply_power()
        res.normalize()
        return res

    def __truediv__(p1_old, p2):
        p1 = deepcopy(p1_old)
        p1.normalize()
        p2.normalize()
        res_coef = np.zeros(len(p1.coefs))

        p1_len = len(p1.coefs)
        p2_len = len(p2.coefs)
        for i in range(p1_len - p2_len, -1, -1):
            val = p1.coefs[i + p2_len - 1] * rev(p2.coefs[-1], Polynom.CUR_MOD)
            res_coef[i] = val
            for j in range(p2_len):
                p1.coefs[j + i] -= val * p2.coefs[j]
                p1.coefs[j + i] %= Polynom.CUR_MOD
        res = Polynom(res_coef)
        res.normalize()
        return res

    def __mod__(p1_old, p2):
        return p1_old - (p1_old / p2) * p2

    def __sub__(p1, p2):
        res_len = max(len(p1.coefs), len(p2.coefs))
        res = Polynom(np.zeros(res_len, dtype=int))
        for i in range(res_len):
            res.coefs[i] = p1.get_value_at(i) - p2.get_value_at(i)
        res.normalize()
        return res

    def shift(self, sh_len):
        if sh_len < 0:
            self.coefs = self.coefs[-sh_len:]
            return
        new_coefs = np.zeros(len(self.coefs) + sh_len, dtype=int)
        new_coefs[sh_len:] = self.coefs
        self.coefs = new_coefs
        return

    def __str__(self):
        res = ""
        res += "Len = " + str(len(self.coefs)) + " Coefs = " + " ".join(str(val) for val in self.coefs) + "\nAs polynom = "
        for i in range(len(self.coefs) - 1, -1, -1):
            if self.coefs[i] != 0:
                if self.coefs[i] > 0:
                    res += "+"
                res += str(self.coefs[i]) + "*x^" + str(i)
        res += "\n"
        return res

    def neg(self):
        res = Polynom()
        res.coefs = np.zeros(Polynom.N, dtype=int)
        res.mod = self.mod
        res.coefs[:len(self.coefs)] = self.coefs
        for i in range(len(res.coefs)):
            res.coefs = res.mod - res.coefs
        return res

    def as_poly_str(self):
        res = ""
        for i in range(len(self.coefs) - 1, -1, -1):
            if self.coefs[i] != 0:
                if self.coefs[i] > 0:
                    res += "+"
                res += str(self.coefs[i]) + "*x^" + str(i)
        res += "\n"
        return res


def modpow(x, pow, mod):
    res = 1
    while pow > 0:
        if pow % 2 == 1:
            res = (res * x) % mod
        x = (x * x) % mod
        pow /= 2
    return res


# reverse int by prime modulo
def rev(x, mod):
    return modpow(x, mod - 2, mod)


def ext_Euclid(x: Polynom, y: Polynom):
    print("Euclid\nx =   " + str(x.coefs) + "\ny =   " + str(y.coefs) + "\nmod = " + str((y % x).coefs))
    x.normalize()
    if len(x.coefs) == 1 and x.coefs[0] == 0:
        a = Polynom((0,))
        b = Polynom((1,))
        return y, a, b

    d, a1, b1 = ext_Euclid(y % x, x)
    a = b1 - (y / x) * a1
    a.apply_mod()
    b = a1
    return d, a, b


def inv(p1: Polynom):
    poly = np.zeros(Polynom.N + 1, dtype=int)
    poly[0] = -1
    poly[-1] = 1
    global_mod = Polynom(poly)  # x^11 - 1

    global_mod.apply_mod()
    Polynom.AUTO_POW_APPLY = False
    d, a, b = ext_Euclid(p1, global_mod)
    Polynom.AUTO_POW_APPLY = True

    d.normalize()
    if len(d.coefs) != 1:
        return None

    return a * rev(d[0])


def is_prime(n):
    for i in range(2, int(n ** 0.5) + 1):
        if n % i == 0:
            return False
    return True


def is_2_power(n):
    return n != 0 and (n & (n - 1) == 0)


def invert_poly(f_poly, R_poly, p):
    inv_poly = None
    if is_prime(p):
        inv_poly = invert(f_poly, R_poly, domain=GF(p))
    elif is_2_power(p):
        inv_poly = invert(f_poly, R_poly, domain=GF(2))
        e = int(math.log(p, 2))
        for i in range(1, e):
            inv_poly = ((2 * inv_poly - f_poly * inv_poly ** 2) % R_poly).trunc(p)
    else:
        raise Exception("Cannot invert polynomial in Z_{}".format(p))
    return inv_poly


def invert_mine(poly: Polynom):
    f_poly = Poly(poly.as_poly_str())
    tmp = "x**" + str(Polynom.N) + "-1"
    R_poly = Poly(tmp)
    inv = invert_poly(f_poly, R_poly, Polynom.CUR_MOD)
    coefs = np.asarray(inv.all_coeffs())[::-1]
    res = Polynom(coefs)
    res.apply_mod()
    return res
