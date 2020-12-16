#include <iostream>
#include "Rsa.h"

using namespace std;

int main()
{
    RSA rsa = GenerateKeys();

    BigNum text("13483148314831849481394811231232121");
    BigNum cipher_text = EncryptRSA(text, rsa.public_key);
    BigNum decrypted_text = DecryptRSA(text, rsa.private_key);

    cout << "text: ";
    text.output();
    cout << "cipher_text: ";
    cipher_text.output();
    cout << "decrypted_text: ";
    decrypted_text.output();

    return 0;
}
