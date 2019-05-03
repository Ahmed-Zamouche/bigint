#include "bigint.hpp"

#include <iostream>
#include <iomanip>
#include <random>
#include <ctime>
#include <functional> //for std::function
#include <algorithm>  //for std::generate_n
#include <ctime>
#include <chrono>

using namespace std;
using namespace bigint;

static std::random_device rd;
static std::mt19937 gen(rd());

int64_t random_int64()
{
    static std::uniform_int_distribution<int64_t> dist(INT64_MIN, INT64_MAX);
    return dist(gen);
}

std::string random_hex(size_t length)
{
    static std::string const default_chars = "0123456789abcdef";
    static std::uniform_int_distribution<size_t> dist{0, default_chars.length() - 1};

    std::string ret;
    std::generate_n(std::back_inserter(ret), length, [&] { return default_chars[dist(gen)]; });
    return ret;
}

double example(unsigned int num)
{
    uint64_t count = 0;
    BigInt c = BigInt(random_hex(1024), 16) * (random_int64() > 0 ? 1 : -1);
    BigInt d = BigInt(random_hex(1024), 16) * (random_int64() > 0 ? 1 : -1);
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < num; i++)
    {

        BigInt m = (c * d);
    }
    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    std::chrono::microseconds duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    count += duration.count();
    cout << "Execution time: " << num << " BigInt Mul in " << (double)count / 1e6 << "s. " << (double)count / num << "us per BigInt Mul" << endl;
    return (double)count / num;
}

int main(int argc, char const *argv[])
{

    gen.seed(time(0));
#if 1
    BigInt a(random_int64());
    BigInt b(random_int64());
    BigInt c = BigInt(random_hex(1024), 16) * (random_int64() > 0 ? 1 : -1);
    BigInt d = BigInt(random_hex(1024), 16) * (random_int64() > 0 ? 1 : -1);

    BigInt e = BigInt(random_hex(11), 16) * (random_int64() > 0 ? 1 : -1);
    BigInt f = BigInt(random_hex(9), 16) * (random_int64() > 0 ? 1 : -1);
#else
    BigInt a = -BigInt(0x6114e97b);
    BigInt b = BigInt(0x9a85c466);
    BigInt c("9f87de166c3fe88634f99f9b7fc7beb008e24b176b0985b0c10fd250ae312be7", 16);
    BigInt d("e2aef871c2049f2688f1b70a304fc492e4a950aded7df55b54f3c98152d02ea7", 16);
#endif

    cout << "const assert = require('assert');" << endl;
    cout << "assert(((" << a << ")*(" << c << "))==" << (a * c) << ");" << endl;
    cout << "assert(((" << c << ")*(" << d << "))==" << (c * d) << ");" << endl;
    example(128);
    return 0;
    cout << "assert(((" << a << ")+(" << b << "))==" << (a + b) << ");" << endl;
    cout << "assert(((" << a << ")-(" << b << "))==" << (a - b) << ");" << endl;
    cout << "assert(((" << c << ")+(" << d << "))==" << (c + d) << ");" << endl;
    cout << "assert(((" << c << ")-(" << d << "))==" << (c - d) << ");" << endl;
    cout << "assert(((" << a << ")+(" << e << "))==" << (a + e) << ");" << endl;
    cout << "assert(((" << e << ")+(" << a << "))==" << (e + a) << ");" << endl;
    cout << "assert(((" << a << ")-(" << f << "))==" << (a - f) << ");" << endl;
    cout << "assert(((" << f << ")-(" << a << "))==" << (f - a) << ");" << endl;
    cout << "assert(((" << a << ")+(" << c << "))==" << (a + c) << ");" << endl;

    return 0;
    cout << "assert(((" << a << ")&(" << b << "))==" << (a & b) << ");" << endl;
    cout << "assert(((" << b << ")&(" << a << "))==" << (b & a) << ");" << endl;
    cout << "assert(((" << c << ")&(" << d << "))==" << (c & d) << ");" << endl;
    cout << "assert(((" << d << ")&(" << c << "))==" << (d & c) << ");" << endl;
    cout << "assert(((" << a << ")&(" << e << "))==" << (a & e) << ");" << endl;
    cout << "assert(((" << e << ")&(" << a << "))==" << (e & a) << ");" << endl;
    cout << "assert(((" << a << ")&(" << f << "))==" << (a & f) << ");" << endl;
    cout << "assert(((" << f << ")&(" << a << "))==" << (f & a) << ");" << endl;
    cout << "assert(((" << a << ")&(" << c << "))==" << (a & c) << ");" << endl;

    cout << "assert(((" << a << ")|(" << b << "))==" << (a | b) << ");" << endl;
    cout << "assert(((" << b << ")|(" << a << "))==" << (b | a) << ");" << endl;
    cout << "assert(((" << c << ")|(" << d << "))==" << (c | d) << ");" << endl;
    cout << "assert(((" << d << ")|(" << c << "))==" << (d | c) << ");" << endl;

    cout << "assert(((" << a << ")|(" << e << "))==" << (a | e) << ");" << endl;
    cout << "assert(((" << e << ")|(" << a << "))==" << (e | a) << ");" << endl;
    cout << "assert(((" << a << ")|(" << f << "))==" << (a | f) << ");" << endl;
    cout << "assert(((" << f << ")|(" << a << "))==" << (f | a) << ");" << endl;

    cout << "assert(((" << a << ")|(" << c << "))==" << (a | c) << ");" << endl;
    return 0;
}
