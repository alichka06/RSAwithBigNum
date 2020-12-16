#include <string>
#include <iostream>
#include <ctime>
#include <random>
#include "Rsa.h"
#include "Prime.h"

Key::Key() {}

Key::Key(const BigNum& v, const BigNum& n) {
    value = v;
    N = n;
}

Key::Key(const Key& k) {
    value = k.value;
    N = k.N;
};

RSA::RSA() {}
RSA::RSA(const Key& _public_key, const PrivateKey& _private_key) {
    private_key = _private_key;
    public_key = _public_key;
}

namespace {
    const int len = 30;
    mt19937 random(static_cast<unsigned int>(time(0)));

    int getRandomNumber(int _min, int _max) {
        static const double fraction = 1.0 / (static_cast<double>(random.max()) + 1.0);
        return static_cast<int>(random() * fraction * (_max - _min + 1) + _min);
    }

    BigNum generateWithLimit(const BigNum& limit) {
        string s;
        bool isless = false;
        int length = limit.length();
        for (int i = 0; i < length; i++) {
            int r, from = 0, to = 9;
            if (i == 0 ) {
                from = 1;
            }
            if (!isless) {
                do {
                    r = getRandomNumber(from, to);
                } while( r > limit.getDigits()[length-i]);
                if (r < limit.getDigits()[length-i]) {
                    isless = true;
                }
            }
            s += to_string(r);
        }
        if (!isless) {
            return BigNum(s)-BigNum(1);
        }
        return BigNum(s);
    }

    BigNum generateBigNum(int length) {
        string s = to_string(getRandomNumber(1,9));
        for (int i = 0; i < length; i++) {
            s += to_string(getRandomNumber(0,9));
        }
        return BigNum(s);
    }

    BigNum generatePrime() {
        BigNum p;
        do {
            p = generateBigNum(len);
        } while (!Miller_Rabin(p, BigNum(10)));
        return p;
    }

    BigNum findE(const BigNum&  phi) {
        return BigNum(3);

        int staticE[] = {3, 17, 65537};
        for (int e: staticE) {
            if (gcd(e, phi) == BigNum(1)) {
                return e;
            }
        }

        BigNum genE = generateWithLimit(phi);
        while (!(gcd(genE, phi) == BigNum(1))) {
            genE = genE + BigNum(1);
        }
        return genE;
    }

    BigNum findD(const BigNum& e, const BigNum&  phi) {
        BigNum x1, y1;
        gcdEv(e, phi, x1, y1);
        return x1;
    }
}

RSA GenerateKeys() {
    BigNum p = generatePrime();
    BigNum q = generatePrime();
    BigNum n = p * q;
    BigNum phi = (q - BigNum(1))*(p - BigNum(1));
    BigNum e = findE(phi);
    BigNum d = findD(e, phi);
    if (d < 0) {
        d = d + phi;
    }

    Key public_key(e, n);

    PrivateKey private_key;
    private_key.d = d;
    private_key.p = p;
    private_key.q = q;

    RSA keys_pair;
    keys_pair.public_key = public_key;
    keys_pair.private_key = private_key;

    return keys_pair;
}

BigNum EncryptRSA(const BigNum& text, const Key& key) {
    return binPowMod(text, key.value, key.N);
}

BigNum DecryptRSA(const BigNum& text, const PrivateKey& key) {
    // https://en.wikipedia.org/wiki/RSA_(cryptosystem)#Using_the_Chinese_remainder_algorithm
    BigNum dp = key.d % (key.p - BigNum(1));
    BigNum dq = key.d % (key.q - BigNum(1));

    BigNum q_inv, t;
    gcdEv(key.q, key.p, q_inv, t);

    BigNum m1 = binPowMod(text, dp, key.p);
    BigNum m2 = binPowMod(text, dq, key.q);
    BigNum h = (q_inv*(m1-m2)) % key.p;
    return (m2 + h * key.q) % (key.p * key.q);
}
