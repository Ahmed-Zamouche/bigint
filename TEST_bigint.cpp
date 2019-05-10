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

double TEST_measure_mul_time(unsigned int num)
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

void TEST_mul(void)
{
    const size_t max_length = 256 * 1024 * sizeof(digit_t);
    const size_t alength = (std::abs(random_int64()) + 1) % max_length;
    const size_t blength = (std::abs(random_int64()) + 1) % max_length;
    BigInt a = BigInt(random_hex(alength * 2), 16) * (random_int64() > 0 ? 1 : -1);
    BigInt b = BigInt(random_hex(blength * 2), 16) * (random_int64() > 0 ? 1 : -1);

    cout << "a = " << a << ";" << endl;
    cout << "b = " << b << ";" << endl;
    BigInt c = (a * b);
    cout << "c = " << c << ";" << endl;
    return;
    cout << "import time;\n\
start = time.time();\n\
cc = a * b;\n\
end = time.time();\n\
if cc == c:\n\
\tprint('Execution time: ', end - start);\n\
else:\n\
\tassert(1==0);\n"
         << endl;
}

void TEST_shift(void)
{
    BigInt e = BigInt(random_hex(8 * sizeof(digit_t) * 2), 16) * (random_int64() > 0 ? -1 : -1);
    for (size_t i = 0; i < 256; i++)
    {
        BigInt f(e << i);
        cout << "assert( hex((" << e << " << 0x" << i << ") == (" << f << ")))" << endl;
    }
    e = BigInt(random_hex(8 * sizeof(digit_t) * 2), 16) * (random_int64() > 0 ? -1 : -1);

    for (size_t i = 0; i < 256; i++)
    {
        BigInt f(e >> i);
        cout << "assert( hex((" << e << " >> 0x" << i << ") == (" << f << ")))" << endl;
    }
}

double TEST_measure_div_time(unsigned int num)
{
    uint64_t count = 0;
    BigInt c = BigInt(random_hex(32 * sizeof(digit_t) * 2), 16) * (random_int64() > 0 ? 1 : -1);
    BigInt d = BigInt(random_hex(1 * sizeof(digit_t) * 2), 16) * (random_int64() > 0 ? 1 : -1);
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < num; i++)
    {

        BigInt m = (c / d);
    }
    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    std::chrono::microseconds duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    count += duration.count();
    cout << "Execution time: " << num << " BigInt Div in " << (double)count / 1e6 << "s. " << (double)count / num << "us per BigInt Mul" << endl;
    return (double)count / num;
}
void TEST_div(void)
{
    const size_t max_length = 32 * sizeof(digit_t);
#if 0
    const size_t nlength = (std::abs(random_int64()) + 1) % max_length;
    const size_t dlength = (std::abs(random_int64()) + 1) % max_length;

    BigInt n = BigInt(random_hex(nlength * 2), 16) * (random_int64() > 0 ? 1 : -1);
    BigInt d = BigInt(random_hex(dlength * 2), 16) * (random_int64() > 0 ? 1 : -1);

#else
    BigInt n = -BigInt("87547ef987af686e18947b62d9e817d82bad3fb870ec8afb9f44f9c3853090b300ca7074e7c8ef3a7ab27b", 16);
    BigInt d = -BigInt("2c156d0dc8f5883eb49e61f76d512e93b414327e578d9bbb33cc5b59448d369f1706ecc2836d445a12715861d875b9c752ba52c5cfde84462a5141d81ac26b4d5c3b7367f8903cdad5e96e", 16);
#endif
    cout << "n = " << n << ";" << endl;
    cout << "d = " << d << ";" << endl;
    BigInt q = (n / d);
    cout << "q = " << q << ";" << endl;
    BigInt r = (n % d);
    cout << "r = " << r << ";" << endl;
    cout << "import time;\n\
start = time.time();\n\
qq = n // d;\n\
end = time.time();\n\
rr = n % d;\n\
if qq == q and rr==r:\n\
\tprint('Execution time: ', end - start);\n\
else:\n\
\tassert(1==0);\n"
         << endl;
}
int main(int argc, char const *argv[])
{
    (void)argc;
    (void)argv;
    gen.seed(time(0));

    //TEST_mul();
    TEST_div();
    //TEST_shift();
    return 0;
}
