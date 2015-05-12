#!/usr/bin/python
import sys

def parse_line(text):
    res = []

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

    print(text, "-->", res)
    final = ''.join((chr(int(n)) for n in res))
    return final

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

def decrypt(p, q, n, b, y):
    a = modinv(b, n) % ((p-1) * (q-1))
    print(a)

    if type(y) is list:
        for line in y:
            print(parse_line(str(pow(int(line), a, n))))
    else:
        print("{} ** {} % {} = {}".format(int(y), a, n, pow(int(y), a, n)))
        print(pow(int(y), a, n))
        (parse_line(str(pow(int(y), a, n))))

def encrypt(n, a, x):
    return pow(x, a, n)

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
                print('regular line', line.strip())
                ciphertext.append(line.strip())

    if None in (p, q, b):
        raise ValueError
    if n is None:
        n = p * q

    return p, q, n, b, ciphertext

if __name__ == "__main__":
    decrypt(*load(sys.argv[1]))
