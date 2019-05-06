#include "bigint.hpp"

#include <cctype>
#include <iomanip>
#include <string>
#include <algorithm>
#include <cassert>

namespace bigint
{

BigInt::BigInt()
{
    this->sign = BigInt::SIGN_POS;
}

BigInt::BigInt(ddigit_t v)
{
    this->sign = v >= 0 ? BigInt::SIGN_POS : BigInt::SIGN_NEG;
    if (v != 0)
    {
        this->numeral.push_back(std::abs(v));
    }
}
BigInt::BigInt(std::string s, int base)
{
    this->sign = BigInt::SIGN_POS;
    //remove leading zeros
    s.erase(0, s.find_first_not_of('0'));

    // return type of stoul is unsigned long
    assert(sizeof(unsigned long) == sizeof(digit_t));

    size_t length = s.size();
    for (size_t i = 8; i <= length; i += 8)
    {
        std::string v_str = s.substr(length - i, 8);
        digit_t v = (digit_t)stoul(v_str, 0, base);
        this->numeral.push_back(v);
    }
    if (length % 8)
    {
        std::string v_str = s.substr(0, length % 8);
        digit_t v = (digit_t)stoul(v_str, 0, base);
        this->numeral.push_back(v);
    }
}

BigInt::BigInt(std::string s) : BigInt(s, 10)
{
}

BigInt::BigInt(const BigInt &o)
{
    this->sign = o.sign;
    this->numeral = o.numeral;
}

BigInt::~BigInt()
{
}

ddigit_t BigInt::cmp(const BigInt &l, const BigInt &r)
{
    if (l.sign != r.sign)
    {
        return l.sign;
    }
    //Assuming that no leading zero are allowed.
    if (l.numeral.size() != r.numeral.size())
    {
        return (l.sign) * ((ddigit_t)l.numeral.size() - (ddigit_t)r.numeral.size());
    }

    size_t length = l.numeral.size();
    for (size_t i = length - 1; i < length; i--)
    {

        if (l.numeral[i] != r.numeral[i])
        {
            return l.sign * ((ddigit_t)l.numeral[i] - (ddigit_t)r.numeral[i]);
        }
    }
    //both l and r are equal
    return 0;
}

BigInt BigInt::complement(const BigInt &o)
{
    return ((BigInt(1) << (o.numeral.size() * BigInt::DIGIT_BIT)) + o);
}

inline std::pair<digit_t, digit_t> BigInt::add(digit_t a, digit_t b, digit_t d)
{
    uddigit_t s = ((uddigit_t)a + b) + d;
    return std::make_pair(s & BigInt::DIGIT_MAX, s > BigInt::DIGIT_MAX);
}
inline std::pair<digit_t, digit_t> BigInt::sub(digit_t a, digit_t b, digit_t d)
{
    ddigit_t s = ((ddigit_t)a - b) - d;
    return std::make_pair(s >= 0 ? s : ((BigInt::DIGIT_MAX + 1) << 1) + s, s < 0);
}

BigInt &BigInt::operator+=(const BigInt &o)
{

    if (this->sign != o.sign)
    {
        BigInt tmp(o);
        tmp.sign = -tmp.sign;
        return operator-=(tmp);
    }

    size_t n = std::min(this->numeral.size(), o.numeral.size());
    size_t m = std::max(this->numeral.size(), o.numeral.size());

    //std::vector<digit_t> sum(m, 0);

    digit_t c = 0; //carry

    for (size_t i = 0; i < n; i++)
    {
        std::tie(this->numeral[i], c) = BigInt::add(this->numeral[i], o.numeral[i], c);
    }

    if (this->numeral.size() > n)
    {
        for (size_t i = n; i < m && c; i++)
        {
            std::tie(this->numeral[i], c) = BigInt::add(this->numeral[i], 0, c);
        }
    }
    else
    {
        for (size_t i = n; i < m; i++)
        {
            digit_t sum;
            std::tie(sum, c) = BigInt::add(0, o.numeral[i], c);
            this->numeral.push_back(sum);
        }
    }

    if (c != 0)
    {
        this->numeral.push_back(c);
    }

    return *this;
}

BigInt &BigInt::operator-=(const BigInt &o)
{
    if (this->sign != o.sign)
    {
        BigInt tmp(o);
        tmp.sign = -tmp.sign;
        return operator+=(tmp);
    }

    //compare absolute values
    ddigit_t d = this->sign * BigInt::cmp(*this, o);
    if (d < 0)
    {

        this->numeral = std::move((o - *this).numeral);
        this->sign = -this->sign;
        return *this;
    }
    else if (d == 0)
    {
        this->numeral.clear();
        return *this;
    }

    size_t n = o.numeral.size();     //min
    size_t m = this->numeral.size(); //max

    digit_t b = 0; //borrow
    for (size_t i = 0; i < n; i++)
    {
        std::tie(this->numeral[i], b) = BigInt::sub(this->numeral[i], o.numeral[i], b);
    }
    for (size_t i = n; i < m && b; i++)
    {
        std::tie(this->numeral[i], b) = BigInt::sub(this->numeral[i], 0, b);
    }

    this->trim();

    return *this;
}
BigInt BigInt::baseMul(const BigInt &l, const BigInt &r)
{
    size_t lsize = l.numeral.size();
    size_t rsize = r.numeral.size();

    BigInt mul;
    mul.numeral = std::vector<digit_t>(lsize + rsize, 0);

    for (size_t i = 0; i < lsize; i++)
    {
        uddigit_t carry = 0;
        for (size_t j = 0; j < rsize; j++)
        {
            uddigit_t sum = (uddigit_t)l.numeral[i] * r.numeral[j] + (mul.numeral[i + j] + carry);
            mul.numeral[i + j] = sum & BigInt::DIGIT_MAX;
            carry = (sum >> BigInt::DIGIT_BIT);
        }
        if (carry != 0)
        {
            mul.numeral[i + r.numeral.size()] = carry;
        }
    }
    mul.sign = l.sign * r.sign;

    while (mul.numeral.back() == 0)
    {
        mul.numeral.pop_back();
    }
    return mul;
}
BigInt &BigInt::baseMul(const BigInt &o)
{
    this->numeral = std::move(BigInt::baseMul(*this, o).numeral);
    return *this;
}

BigInt BigInt::karatsubaMul(BigInt &l, BigInt &r)
{
    size_t n = l.numeral.size();
    size_t m = r.numeral.size();

    if (n == 0 || m == 0)
    {
        return 0;
    }
    if ((n + m) < (2 * BigInt::KARATSUBA_THRESHOLD))
    {
        return BigInt::baseMul(l, r);
    }

    if (n < m)
    {
        l.pad(m - n);
        n = m;
    }
    else
    {
        r.pad(n - m);
        m = n;
    }

    if ((n % 2) == 1)
    {
        l.numeral.push_back(0);
        r.numeral.push_back(0);
        n++;
    }

    size_t k = n / 2;

    BigInt l0, l1, l2;
    BigInt r0, r1, r2;

    l0.numeral.assign(l.numeral.begin() + 0, l.numeral.begin() + k);
    l1.numeral.assign(l.numeral.begin() + k, l.numeral.begin() + n);

    r0.numeral.assign(r.numeral.begin() + 0, r.numeral.begin() + k);
    r1.numeral.assign(r.numeral.begin() + k, r.numeral.begin() + n);

    l2 = l0 - l1;
    r2 = r0 - r1;

    int sign = -(l2.sign * r2.sign);

    BigInt c0 = BigInt::karatsubaMul(l0, r0);
    BigInt c1 = BigInt::karatsubaMul(l1, r1);
    BigInt c2 = BigInt::karatsubaMul(l2.abs(), r2.abs());

    c2.sign = sign;

    BigInt c;
    c += c0;
    c += (c0 + c1 + c2) << (k * BigInt::DIGIT_BIT);
    c += c1 << (2 * k * BigInt::DIGIT_BIT);
    return c;
}

BigInt &BigInt::karatsubaMul(const BigInt &o)
{
    BigInt r(o);
    this->numeral = std::move(BigInt::karatsubaMul(*this, r).numeral);
    return *this;
}

BigInt &BigInt::operator*=(const BigInt &o)
{
    return this->karatsubaMul(o);
}

BigInt &BigInt::operator/=(const BigInt &o) { return *this; }
BigInt &BigInt::operator%=(const BigInt &o) { return *this; }

BigInt &BigInt::operator&=(const BigInt &o)
{

    BigInt l = BigInt(*this);
    BigInt r = BigInt(o);
    if (l.sign == BigInt::SIGN_NEG)
    {
        l.numeral = std::move((BigInt::complement(l).numeral));
    }
    if (r.sign == BigInt::SIGN_NEG)
    {
        r.numeral = std::move((BigInt::complement(r).numeral));
    }

    size_t n = std::min(l.numeral.size(), r.numeral.size());
    for (size_t i = 0; i < n; i++)
    {
        l.numeral[i] &= r.numeral[i];
    }
    if (this->sign == BigInt::SIGN_NEG && o.sign == BigInt::SIGN_NEG)
    {
        l.sign = BigInt::SIGN_NEG;
        this->sign = BigInt::SIGN_NEG;
        this->numeral = std::move((BigInt::complement(l).numeral));
    }
    else
    {
        this->numeral = std::move(l.numeral);
        this->sign = BigInt::SIGN_POS;
    }
    this->numeral.erase(this->numeral.cbegin() + n, this->numeral.cend());
    return *this;
}

BigInt &BigInt::operator|=(const BigInt &o)
{
    BigInt l = BigInt(*this);
    BigInt r = BigInt(o);
    if (l.sign == BigInt::SIGN_NEG)
    {
        l.numeral = std::move((BigInt::complement(l).numeral));
    }
    if (r.sign == BigInt::SIGN_NEG)
    {
        r.numeral = std::move((BigInt::complement(r).numeral));
    }

    size_t n = std::min(l.numeral.size(), r.numeral.size());

    for (size_t i = 0; i < n; i++)
    {
        l.numeral[i] |= r.numeral[i];
    }

    if (l.numeral.size() < r.numeral.size())
    {
        for (size_t i = n; i < r.numeral.size(); i++)
        {
            l.numeral.push_back(r.numeral[i]);
        }
    }
    //FIXME:
    if (this->sign == BigInt::SIGN_NEG || o.sign == BigInt::SIGN_NEG)
    {
        l.sign = BigInt::SIGN_NEG;
        this->sign = BigInt::SIGN_NEG;
        this->numeral = std::move((BigInt::complement(l).numeral));
    }
    else
    {
        this->numeral = std::move(l.numeral);
        this->sign = BigInt::SIGN_POS;
    }

    this->numeral.erase(this->numeral.cbegin() + n, this->numeral.cend());

    return *this;
}
BigInt &BigInt::operator^=(const BigInt &o)
{
    //FIXME::
    size_t n = std::min(this->numeral.size(), o.numeral.size());

    for (size_t i = 0; i < n; i++)
    {
        this->numeral[i] ^= o.numeral[i];
    }
    this->numeral.erase(this->numeral.cbegin() + n, this->numeral.cend());
    return *this;
}

BigInt &BigInt::operator<<=(const BigInt &o)
{
    if (o.sign == BigInt::SIGN_NEG)
    {
        BigInt tmp(o);
        tmp.sign = -tmp.sign;
        return this->operator<<=(tmp);
    }

    if (*this == 0 || o == 0)
    {
        return *this;
    }

    //RangeError: Maximum BigInt size exceeded
    assert(o.numeral.size() < 2);

    digit_t lshift;

    lshift = (o.numeral[0] & ~(BigInt::DIGIT_BIT - 1));
    if (lshift != 0)
    {
        this->numeral.insert(this->numeral.begin(), (lshift / BigInt::DIGIT_BIT), 0);
    }

    lshift = (o.numeral[0] & (BigInt::DIGIT_BIT - 1));

    if (lshift != 0)
    {

        size_t length = this->numeral.size();

        digit_t msb = this->numeral[length - 1] >> (BigInt::DIGIT_BIT - lshift); // most significant bits
        for (size_t i = length - 1; i ; i--)
        {
            this->numeral[i] = this->numeral[i] << lshift;
            this->numeral[i] |= this->numeral[i - 1] >> (BigInt::DIGIT_BIT - lshift);
        }
        this->numeral[0] <<= lshift;
        if (msb != 0)
        {
            this->numeral.push_back(msb);
        }
    }

    return *this;
}

BigInt &BigInt::operator>>=(const BigInt &o)
{
    if (o.sign == BigInt::SIGN_NEG)
    {
        BigInt tmp(o);
        tmp.sign = -tmp.sign;
        return this->operator>>=(tmp);
    }

    if (*this == 0 || o == 0)
    {
        return *this;
    }

    //RangeError: Minimum BigInt size exceeded
    assert(o.numeral.size() < 2);

    digit_t rshift;
    rshift = (o.numeral[0] & ~(BigInt::DIGIT_BIT - 1));
    if (rshift != 0)
    {
        if ((rshift / BigInt::DIGIT_BIT) >= this->numeral.size())
        {
            this->numeral.clear();
            return *this;
        }
        this->numeral.erase(this->numeral.begin(), this->numeral.begin() + (rshift / BigInt::DIGIT_BIT));
    }

    rshift = (o.numeral[0] & (BigInt::DIGIT_BIT - 1));

    if (rshift != 0)
    {
        size_t length = this->numeral.size();
        for (size_t i = 0; i < length - 1; i++)
        {
            this->numeral[i] = this->numeral[i] >> rshift;
            this->numeral[i] |= this->numeral[i + 1] << (BigInt::DIGIT_BIT - rshift);
        }

        this->numeral[length - 1] >>= rshift;
        this->trim();
    }
    return *this;
}

BigInt &BigInt::operator++()
{
    *this += BigInt(1);
    return *this;
}
BigInt &BigInt::operator--()
{
    *this -= BigInt(1);
    return *this;
}

std::ostream &operator<<(std::ostream &os, const BigInt &a)
{

    size_t length = a.numeral.size();
    if (length != 0)
    {
        if (a.sign == BigInt::SIGN_NEG)
        {
            os << '-';
        }

        os << "0x";

        os << std::hex << a.numeral[length - 1];
        for (size_t i = length - 2; i < length; i--)
        {
            auto digit = a.numeral[i];
            os << std::setw(8) << std::setfill('0') << digit;
            //os << digit;
        }
        //os << '[' << std::dec << length << ']';
        os << 'n';
    }
    else
    {
        os << '0';
    }

    return os;
}

} // namespace bigint
