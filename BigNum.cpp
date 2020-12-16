#include "Bignum.h"
#include <iostream>
#include <algorithm>
using namespace std;
BigNum::BigNum(const BigNum& a){
    sign = a.getSign();
    digits = a.digits;
}
BigNum::BigNum(string str){
    sign = 1;
    digits.clear();
    for (int j = str.length()-1; j>0; j--){
        digits.push_back((int)(str[j]-48));
    }
    if (str[0] == '-')
        sign = -1;
    else digits.push_back((int)(str[0]-48));
}
BigNum::BigNum(const int a){
    int k = a;
    if (k >= 0) sign = 1; else sign = -1;
    k = k*sign;
    digits.clear();
    if (k == 0)
        digits.push_back(0);
    while(k > 0){
        digits.push_back(k%10);
        k = k/10;
    }
}
int BigNum::length() const{
    return digits.size();
}
int BigNum::getSign() const{
    return sign;
}
vector<int> BigNum::getDigits() const {
    return digits;
};

int BigNum::compareByModule(const BigNum& b) const{
    if (length()>b.length()) return 1; //перше більше
    if (length()<b.length()) return -1; //друге більше
    int i = length();
    while (i > 0 && digits[i-1] == b.digits[i-1])
        i--;
    if (i == 0) return 0;
    if (digits[i-1] > b.digits[i-1])
        return 1;
    else
        return -1;
}
void checkNulls(vector<int>& a){
    while (a[a.size()-1] == 0)
        a.pop_back();
    if (a.empty())
        a.push_back(0);
}
BigNum& BigNum::operator=(const BigNum& b){
    sign = b.sign;
    digits.clear();
    for (int i=0; i<b.length(); i++)
        digits.push_back(b.digits[i]);
    return *this;
}
bool operator==(const BigNum& a, const BigNum& b){
    if (a.getSign() != b.getSign())
        return false;
    return a.compareByModule(b) == 0;
}
bool operator >(const BigNum& a, const BigNum& b){
    if (a.getSign() > b.getSign())
        return true;
    if (b.getSign() > a.getSign())
        return false;
    return (a.getSign() == -1 && a.compareByModule(b) == -1) ||
           (a.getSign() == 1 && a.compareByModule(b) == 1);
}
bool operator <(const BigNum& a, const BigNum& b){
    if (a.getSign() > b.getSign())
        return false;
    if (b.getSign() > a.getSign())
        return true;
    return (a.getSign() == -1 && a.compareByModule(b) == 1) ||
           (a.getSign() == 1 && a.compareByModule(b) == -1);
}
const BigNum operator+(const BigNum& a, const BigNum& b){
    BigNum res;
    BigNum a1(a);
    BigNum b1(b);
    if (a1.sign < 0 && b1.sign > 0){
        a1.sign = 1;
        return b1 - a1;
    }
    if (a1.sign > 0 && b1.sign < 0){
        b1.sign = 1;
        return a1 - b1;
    }
    res.sign = a.sign;
    res.digits = addVector(a.digits, b.digits);
    return res;
}

vector<int> addVector(const vector<int>& a, const vector<int>& b){
    vector<int> ans;
    ans.resize(max(a.size(),b.size())+1);
    for (int i=0; i < a.size(); i++)
        ans[i] = a[i];
    for (int i=0; i < b.size(); i++){
        ans[i] += b[i];
        ans[i+1] += ans[i]/10;
        ans[i] = ans[i]%10;
        int k = i+1;
        while (ans[k]>=10){
            ans[k+1] += ans[k]/10;
            ans[k] %= 10;
            k++;
        }
    }
    checkNulls(ans);
    return ans;
}
void BigNum::output() const{
    if (sign < 0)
        cout << '-';
    for (int i=length()-1; i>=0; i--)
        cout << digits[i];
    cout << "\n";
}

const BigNum operator-(const BigNum& a, const BigNum& b){
    BigNum res;
    BigNum a1(a);
    BigNum b1(b);
    if (a1.sign < 0 && b1.sign > 0){
        res.sign = -1;
        res.digits = addVector(a1.digits, b1.digits);
        return res;
    }
    if (b1.sign < 0){
        b1.sign = 1;
        return a1+b1;
    }
    if (a1.compareByModule(b1) == -1){
        res.sign = -1;
        res.digits = subVector(b1.digits, a1.digits);
    } else{
        res.sign = 1;
        res.digits = subVector(a1.digits, b1.digits);
    }
    return res;
}

vector<int> subVector(const vector<int>& a, const vector<int>& b){
    vector<int> res;
    res = a;
    for (int i=0; i < res.size() && i < b.size(); i++){
        if (res[i] < b[i]){
            int k = 1;
            while (res[i+k] == 0)
                k++;
            for (int j = 0; j < k; j++){
                res[i+j+1]--;
                res[i+j] += 10;
            }
        }
        res[i] -= b[i];
    }
    checkNulls(res);
    return res;
}

const BigNum operator*(const BigNum& a, const BigNum& b){
    BigNum res;
    res.sign = a.getSign()*b.getSign();
    res.digits.assign(a.length()+b.length(), 0);
    for (int i = 0; i < b.length(); i++)
        for (int j = 0; j < a.length(); j++){
            int d = a.digits[j]*b.digits[i];
            res.digits[i+j] += d%10;
            res.digits[i+j+1] += d/10;
            int k = i+j;
            while (k < i+j+1 || res.digits[k]>=10){
                res.digits[k+1] += res.digits[k]/10;
                res.digits[k] %= 10;
                k++;
            }
        }
    checkNulls(res.digits);
    return res;
}

void BigNum::levelUp(){
    digits.push_back(0);
    for (int i=digits.size()-1; i>0; i--)
        digits[i] = digits[i-1];
}

const BigNum operator/(const BigNum& a, const BigNum& b){
    BigNum res;
    if (a.compareByModule(b) == -1){
        res.digits.push_back(0);
        return res;
    }
    res.sign = a.sign*b.sign;
    BigNum cur;
    cur.sign = 1;
    BigNum absB(b);
    absB.sign = 1;
    for (int i=a.length()-1; i>=0; --i){
        cur.levelUp();
        cur.digits[0] = a.digits[i];
        checkNulls(cur.digits);
        if (cur < absB){
            if (res.digits.size() > 0) res.digits.push_back(0);
            continue;
        }
        int dig = 0;
        int l = 0, r = 10;
        while (l <= r){
            int m = (l + r) / 2;
            BigNum x(m);
            BigNum t;
            t = x*absB;
            if (!(t>cur)){
                dig = m;
                l = m+1;
            }
            else
                r = m-1;
        };
        res.digits.push_back(dig);
        cur = cur - dig*absB;
    }

    reverse(res.digits.begin(), res.digits.end());
    checkNulls(res.digits);

    BigNum t(0);
    if ((res.sign == -1) && (cur.compareByModule(t)!=0))
        res = res - 1;

    return res;
}

const BigNum operator%(const BigNum& a, const BigNum& b){
    BigNum cur;
    cur.sign = 1;
    BigNum absB(b);
    absB.sign = 1;
    for (int i=a.length()-1; i>=0; --i){
        cur.levelUp();
        cur.digits[0] = a.digits[i];
        checkNulls(cur.digits);
        if (cur < absB){
            continue;
        }
        int dig = 0;
        int l = 0, r = 10;
        while (l <= r){
            int m = (l + r) / 2;
            BigNum x(m);
            BigNum t;
            t = x*absB;
            if (!(t>cur)){
                dig = m;
                l = m+1;
            }
            else
                r = m-1;
        };
        cur = cur - dig*absB;
    }
    if (a.sign == -1) cur = b - cur;
    return cur;
}

const BigNum binPow(const BigNum& a, const BigNum& b){
    if (b == BigNum(0))
        return BigNum(1);
    if (b%2 == 1){
        BigNum x(1);
        x = b - x;
        return binPow(a, x)*a;
    }
    else{
        BigNum x = b/2;
        BigNum t = binPow(a, x);
        return t*t;
    }
}

const BigNum binPowMod(const BigNum& a, const BigNum& b, const BigNum& mod){
    if (b == BigNum(0))
        return BigNum(1);
    if (b%2 == 1){
        BigNum x(1);
        x = b - x;
        return (binPowMod(a, x, mod)*a)%mod;
    }
    else{
        BigNum x = b/2;
        BigNum t = binPowMod(a, x, mod);
        return (t*t)%mod;
    }
}

BigNum BigNum::square() const{
    BigNum l(0);
    BigNum r = *this;
    BigNum res;
    while (!(l>r))
    {
        BigNum m = (l + r);
        BigNum t(2);
        m = m/t;
        if (!(m*m > *this))
        {
            res = m;
            l = m + 1;
        }
        else
            r = m - 1;
    }
    return res;
}

void systemEq(){
    bool flag = true;
    do{
        int n;
        cout << "Enter number of equations: ";
        cin >> n;
        BigNum a[n];
        BigNum p[n];
        for (int i=0; i<n; i++){
            string s;
            cout << i+1 << ":  a - ";
            cin >> s;
            a[i] = BigNum(s);
            cout << "    p - ";
            cin >> s;
            p[i] = BigNum(s);
        }
        if (isCoPrime(p,n))
            flag = false;
        else{
            cout << "Try again\n";
            continue;
        }
        BigNum M(1);
        for (int i=0; i<n; i++)
            M = M*p[i];
        BigNum m[n];
        for (int i=0; i<n; i++)
            m[i] = M / p[i];
        BigNum m_1[n];

        for (int i=0; i<n; i++){
            BigNum x, y;
            if (m[i] < p[i])
                gcdEv(m[i], p[i], x, y);
            else
                gcdEv(p[i], m[i], y, x);
            m_1[i] = x;
        }
        /*for (int i=0; i<n; i++){
            BigNum y(0);
            BigNum t(1);
            while (!( (y*m[i]) % p[i] == t) )
                y = y + 1;
            m_1[i] = y;
        }*/
        BigNum res(0);
        for (int i=0; i<n; i++)
            res = res + a[i]*m[i]*m_1[i];
        cout << "Answer: ";
        res.output();

    }while (flag);
}
bool isCoPrime(BigNum a[], int n){
    for (int i=0; i < n; i++)
        for (int j = i+1; j<n; j++){
            BigNum b = gcd(a[i], a[j]);
            BigNum t(1);
            if (!(b == t)){
                cout << "Not all p are pairwise coprime\n";
                return false;
            }
        }
    return true;
}
BigNum gcd(const BigNum& f, const BigNum& s){
    BigNum a(f);
    BigNum b(s);
    if (a < BigNum(0))
        a = a*BigNum(-1);
    if (b < BigNum(0))
        b = b*BigNum(-1);
    while (!(a==BigNum(0)) && !(b==BigNum(0))){
        if (a > b)
            a = a % b;
        else
            b = b % a;
    }
    BigNum ans;
    if (a>b)
        ans = a;
    else
        ans = b;
    return ans;
}

namespace {
    BigNum _gcdEv(const BigNum& f, const BigNum& s, BigNum& x, BigNum& y){
        if (f == BigNum(0)){
            x = BigNum(0);
            y = BigNum(1);
            return s;
        }
        BigNum a(f), b(s);
        BigNum x1, y1;
        BigNum d = gcdEv(b%a, a, x1, y1);
        x = y1 - x1*(s/f);
        y = x1;
        return d;
    }
}

BigNum gcdEv(const BigNum& f, const BigNum& s, BigNum& x, BigNum& y){
    if (f < s) {
        return _gcdEv(f, s, x, y);
    } else {
        return _gcdEv(s, f, y, x);
    }
}
