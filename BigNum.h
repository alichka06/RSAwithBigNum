#ifndef BIGNUM_H_INCLUDED
#define BIGNUM_H_INCLUDED


#include <vector>
#include <string>
using namespace std;

class BigNum;
const BigNum operator+(const BigNum& a, const BigNum& b);
const BigNum operator-(const BigNum& a, const BigNum& b);
const BigNum operator*(const BigNum& a, const BigNum& b);
const BigNum operator/(const BigNum& a, const BigNum& b);
const BigNum operator%(const BigNum& a, const BigNum& b);
const BigNum binPowMod(const BigNum& a, const BigNum& b, const BigNum& mod);
const BigNum binPow(const BigNum& a, const BigNum& b);
bool operator==(const BigNum& a, const BigNum& b);
bool operator >(const BigNum& a, const BigNum& b);
bool operator <(const BigNum& a, const BigNum& b);

BigNum square(const BigNum& a);
void systemEq();
bool isCoPrime(BigNum a[], int n);
BigNum gcd(const BigNum& f, const BigNum& s);
BigNum gcdEv(const BigNum& f, const BigNum& s, BigNum& x, BigNum& y);

vector<int> subVector(const vector<int>& a, const vector<int>& b);
vector<int> addVector(const vector<int>& a, const vector<int>& b);
void checkNulls(vector<int>& a);

class BigNum{
private:
    int sign;
    vector<int> digits;
public:
    BigNum(){sign = 1; digits.clear();};
    BigNum(const BigNum& a);
    BigNum(string str);
    BigNum(int a);
    int length() const;
    int getSign() const;
    vector<int> getDigits() const;
    int compareByModule(const BigNum& b) const;
    void output() const;
    BigNum& operator= (const BigNum& b);
    friend const BigNum operator+(const BigNum& a, const BigNum& b);
    friend const BigNum operator-(const BigNum& a, const BigNum& b);
    friend const BigNum operator*(const BigNum& a, const BigNum& b);
    friend const BigNum operator/(const BigNum& a, const BigNum& b);
    friend const BigNum operator%(const BigNum& a, const BigNum& b);
    void levelUp();
    BigNum square() const;
};


#endif // BIGNUM_H_INCLUDED
