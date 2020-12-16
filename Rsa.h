#ifndef RSA_H_INCLUDED
#define RSA_H_INCLUDED

#include "BigNum.h"

struct Key {
    BigNum value;
    BigNum N;

    Key();
    Key(const BigNum& v, const BigNum& n);
    Key(const Key& k);
};

struct PrivateKey {
    BigNum p;
    BigNum q;
    BigNum d;
};

struct RSA {
    PrivateKey private_key;
    Key public_key;

    RSA();
    RSA(const Key& _public_key, const PrivateKey& _private_key);
};

RSA GenerateKeys();
BigNum EncryptRSA(const BigNum& text, const Key& key);
BigNum DecryptRSA(const BigNum& text, const PrivateKey& key);


#endif // RSA_H_INCLUDED
