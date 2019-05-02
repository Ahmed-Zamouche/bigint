#include "bigint.hpp"

#include <iostream>
#include <random>
#include <functional> //for std::function
#include <algorithm>  //for std::generate_n

using namespace std;
using namespace bigint;

int64_t random_int64()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<int64_t> dist(INT64_MIN, INT64_MAX);

    return dist(gen);
}

std::string random_hex(size_t length)
{
    static std::string const default_chars = "0123456789abcdef";
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<size_t> dist{0, default_chars.length() - 1};

    std::string ret;

    std::generate_n(std::back_inserter(ret), length, [&] { return default_chars[dist(gen)]; });
    return ret;
}

int main(int argc, char const *argv[])
{
    BigInt a(random_int64());
    BigInt b(random_int64());
    BigInt c(random_hex(64), 16);
    BigInt d(random_hex(64), 16);
    cout << a << '+' << b << '=' << a + b << endl;
    cout << a << '-' << b << '=' << a - b << endl;
    cout << "0x" << c << "n + "
         << "0x" << d << "n == 0x" << c + d << 'n' << endl;
    cout << "0x" << c << "n - "
         << "0x" << d << "n == 0x" << c - d << 'n' << endl;
    return 0;
}
