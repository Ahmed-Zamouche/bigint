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
    BigInt c = BigInt(random_hex(8 * 1024 * sizeof(digit_t) * 2), 16) * (random_int64() > 0 ? 1 : -1);
    BigInt d = BigInt(random_hex(8 * 1024 * sizeof(digit_t) * 2), 16) * (random_int64() > 0 ? 1 : -1);
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

    BigInt a(random_int64());
    BigInt b(random_int64());
    BigInt c = BigInt(random_hex(8 * 1024 * sizeof(digit_t) * 2), 16) * (random_int64() > 0 ? 1 : -1);
    BigInt d = BigInt(random_hex(1024 * sizeof(digit_t) * 2), 16) * (random_int64() > 0 ? 1 : -1);

    BigInt e = BigInt(random_hex(11), 16) * (random_int64() > 0 ? 1 : -1);
    BigInt f = BigInt(random_hex(9), 16) * (random_int64() > 0 ? 1 : -1);
#if 0
    example(128);
#else
    cout << "const a = " << c << ";" << endl;
    cout << "const b = " << d << ";" << endl;
    cout << "const c = " << (c * d) << ";" << endl;

    cout << "const hrstart = process.hrtime();\n\
\n\
const runMul = function(){\n\
    var c = a * b;\n\
    hrend = process.hrtime(hrstart);\n\
    console.info('Execution time (hr): %ds %dus', hrend[0], hrend[1] / 1000);\n\
    return c;\n\
}\n\
console.log(c == runMul());\n"
         << endl;
#endif
    return 0;
    cout << "const assert = require('assert');" << endl;
    cout << "assert(((" << a << ")*(" << c << "))==" << (a * c) << ");" << endl;
    cout << "assert(((" << c << ")*(" << d << "))==" << (c * d) << ");" << endl;
    return 0;
}
