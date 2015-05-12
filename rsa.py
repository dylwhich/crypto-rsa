#!/usr/bin/python
import sys

def parse_line(text):
    res = []

    if type(text) is int:
        text = str(text)
    for char in text:
        if not res:
            res.append(char)
            continue
        cur = res[-1]
        if int(cur+char) < 127:
            res[-1] += char
        else:
            if int(cur) < 32:
                print("Invalid: {}".format(cur))
            res.append(char)

    return ''.join((chr(int(n)) for n in res))

# From rosettacode.org/wiki/Modular_inverse#Python
def extended_gcd(aa, bb):
    last_rem, rem = abs(aa), abs(bb)
    x, last_x, y, last_y = 0, 1, 1, 0
    while rem:
        last_rem, (quotient, rem) = rem, divmod(last_rem, rem)
        x, last_x = last_x - quotient * x, x
        y, last_y = last_y - quotient * y, y
    return last_rem, last_x * (-1 if aa < 0 else 1), last_y * (-1 if bb < 0 else 1)

def modinv(a, m):
    g, x, y = extended_gcd(a, m)
    if g != 1:
        raise ValueError
    return x % m

class RSA:
    def __init__(self, p, q, public_exponent, private_exponent=None):
        self.p = p
        self.q = q
        self.n = p*q
        self.e_pub = public_exponent

        if not private_exponent:
            private_exponent = modinv(public_exponent, (p-1)*(q-1))
        self.e_priv = private_exponent

    def encrypt(self, number):
        return pow(number, self.e_pub, self.n)

    def decrypt(self, number):
        return pow(number, self.e_priv, self.n)

def load(fn):
    # returns p, q, n, b, ciphertext
    p=q=n=b=None
    ciphertext = []

    with open(fn) as f:
        for line in f:
            if '=' in line:
                name, val = [s.strip() for s in line.split('=')]
                if name is 'p':
                    p = int(val)
                elif name is 'q':
                    q = int(val)
                elif name is 'n':
                    n = int(val)
                elif name is 'b':
                    b = int(val)
            elif line.strip():
                ciphertext.append(line.strip())

    if None in (p, q, b):
        raise ValueError
    if n is None:
        n = p * q

    return RSA(p, q, b), ciphertext

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print(""" Usage: {} <cipher-file>

        cipher-file should consist of lines of the format <name>=<val>,
        where name is 'p', 'q', 'n', or 'b', corresponding to the factors of the
        RSA modular value, the RSA modular value, and the public exponent.
        Providing n is optional. Following these lines should be the ciphertext,
        as numbers, one per line.""")
        exit(0)

    crypto, text = load(sys.argv[1])
    print(''.join([parse_line(crypto.decrypt(int(line))) for line in text]))
