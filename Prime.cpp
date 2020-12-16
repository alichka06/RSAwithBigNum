#include "Prime.h"
#include <iostream>
#include <ctime>
using namespace std;

bool Miller_Rabin(const BigNum& n, BigNum k){
    if (n == 2 || n == 3)
        return true;
    if (n % 2 == 0 || n % 3 == 0 || n == 1)
        return false;

    BigNum s, t = n-1;
    while (t % 2 == 0){
        s = s + 1;
        t = t / 2;
    }
    srand( time(0));
    while (k > 0){
        k = k - 1;
        bool flag = true;
        int r = rand();
        BigNum a(r);
        BigNum temp = n - BigNum(3);
        a = (a*temp)/BigNum(32767) + 2;
        BigNum x = binPowMod(a, t, n);
        temp = n - 1;
        if ( x == BigNum(1) || x == temp)
            continue;
        for (BigNum j(1); j < s; j = j + 1) {
            x = (x*x)%n;
            if (x == 1)
                return false;
            if (x == temp){
                flag = false;
                break;
            }
        }
        if (flag) return false;
    }
    return true;
}
