#include "bigint.hpp"

#include <iostream>

using namespace std;
using namespace bigint;

int main(int argc, char const *argv[])
{
    BigInt a(5);
    BigInt b(3);
    BigInt c("f123456789abcdef", 16);
    BigInt d("fedcba9876543210fedcba9876543210", 16);
    BigInt f("fedcba9876543210fedcba9876543210fedcba9876543211", 16);

    BigInt e = ~a;
    cout << "!(" << a << "): " << string(!a ? "true" : "false") << endl;

    cout << a << " < " << b << " : " << string(a < b ? "true" : "false") << endl;
    cout << a << " <= " << b << " : " << string(a <= b ? "true" : "false") << endl;
    cout << a << " > " << b << " : " << string(a > b ? "true" : "false") << endl;
    cout << a << " >= " << b << " : " << string(a >= b ? "true" : "false") << endl;
    cout << a << " > " << c << " : " << string(a > c ? "true" : "false") << endl;
    cout << 5 << " > " << c << " : " << string(5 > c ? "true" : "false") << endl;
    cout << a << " + " << b << " : " << a + b << endl;
    cout << a << " - " << b << " : " << a - b << endl;
    cout << b << " - " << a << " : " << b - a << endl;

    cout << d << " - " << f << " : " << d - f << endl;
    cout << f << " - " << d << " : " << f - d << endl;

    cout << -b << " + " << a << " : " << (-b) + (a) << endl;
    cout << f << " + " << -d << " : " << (f) + (-d) << endl;

    cout << -b << " - " << a << " : " << (-b) - (a) << endl;
    cout << f << " - " << -d << " : " << (f) - (-d) << endl;

    cout << d << " - " << d + 1 << " : " << (d + 1) - d << endl;

    cout << " c = " << c << endl;
    cout << f << " & " << d << " : " << (f & d) << endl;
    cout << f << " & " << c << " : " << (f & c) << endl;

    cout << f << " << " << c << " : " << (f & c) << endl;
    cout << f << " << " << c << " : " << (f & c) << endl;
    cout << f << " << " << c << " : " << (f & c) << endl;
    cout << f << " << " << c << " : " << (f & c) << endl;
    cout << f << " << " << c << " : " << (f & c) << endl;
    cout << f << " << " << c << " : " << (f & c) << endl;
    return 0;
}
