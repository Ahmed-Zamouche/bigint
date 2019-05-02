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

    for (size_t i = 0; i < l.numeral.size(); i++)
    {

        if (l.numeral[i] != r.numeral[i])
        {
            return l.sign * (l.numeral[i] - r.numeral[i]);
        }
    }
    //both l and r are equal
    return 0;
}

BigInt &BigInt::operator+=(const BigInt &o)
{
    if (this->sign != o.sign)
    {
        BigInt tmp(o);
        tmp.sign = -tmp.sign;
        return operator-=(tmp);
    }

    uddigit_t carry = 0;

    size_t n = std::min(this->numeral.size(), o.numeral.size());

    for (size_t i = 0; i < n; i++)
    {
        uddigit_t sum = this->numeral[i] + (o.numeral[i] + carry);
        carry = sum > BigInt::DIGIT_MAX;
        this->numeral[i] = sum & BigInt::DIGIT_MAX;
    }

    ddigit_t dlength = (ddigit_t)this->numeral.size() - (ddigit_t)o.numeral.size();

    if (dlength > 0)
    {
        for (size_t i = n; i < this->numeral.size() && carry; i++)
        {
            uddigit_t sum = this->numeral[i] + carry;
            carry = sum > BigInt::DIGIT_MAX;
            this->numeral[i] = sum & BigInt::DIGIT_MAX;
        }
    }
    else if (dlength < 0)
    {
        for (size_t i = n; i < o.numeral.size(); i++)
        {
            uddigit_t sum = o.numeral[i] + carry;
            carry = sum > BigInt::DIGIT_MAX;
            this->numeral.push_back(sum & BigInt::DIGIT_MAX);
        }
    }
    else
    {
        ;
    }

    if (carry != 0)
    {
        this->numeral.push_back(carry);
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

    uddigit_t borrow = 0;

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

    size_t n = std::min(this->numeral.size(), o.numeral.size());

    for (size_t i = 0; i < n; i++)
    {
        ddigit_t diff = this->numeral[i] - (o.numeral[i] + borrow);
        if (diff < 0)
        {
            diff = ((BigInt::DIGIT_MAX + 1) << 1) + diff;
            borrow = 1;
        }
        else
        {
            borrow = 0;
        }
        this->numeral[i] = diff;
    }
    for (size_t i = n; i < this->numeral.size() && borrow; i++)
    {
        ddigit_t diff = this->numeral[i] - borrow;
        if (diff < 0)
        {
            diff = ((BigInt::DIGIT_MAX + 1) << 1) + diff;
            borrow = 1;
        }
        else
        {
            borrow = 0;
        }
        this->numeral[i] = diff;
    }

    while (this->numeral.back() == 0)
    {
        this->numeral.pop_back();
    }
    return *this;
}

BigInt &BigInt::operator*=(const BigInt &o) { return *this; }
BigInt &BigInt::operator/=(const BigInt &o) { return *this; }
BigInt &BigInt::operator%=(const BigInt &o) { return *this; }

BigInt &BigInt::operator&=(const BigInt &o)
{
    //FIXME::
    size_t n = std::min(this->numeral.size(), o.numeral.size());
    for (size_t i = 0; i < n; i++)
    {
        this->numeral[i] &= o.numeral[i];
    }
    this->numeral.erase(this->numeral.begin() + n, this->numeral.end());
    return *this;
}

BigInt &BigInt::operator|=(const BigInt &o)
{
    //FIXME::
    size_t n = std::min(this->numeral.size(), o.numeral.size());

    for (size_t i = 0; i < n; i++)
    {
        this->numeral[i] |= o.numeral[i];
    }
    this->numeral.erase(this->numeral.begin() + n, this->numeral.end());
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
    this->numeral.erase(this->numeral.begin() + n, this->numeral.end());
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

    if (*this == 0)
    {
        return *this;
    }

    //FIXME::
    BigInt tmp(o);
    while (tmp >= BigInt::DIGIT_BIT)
    {
        size_t length = this->numeral.size();

        digit_t msd = this->numeral[length - 1]; // most significant digit
        for (size_t i = length - 1; i < length - 1; i--)
        {
            this->numeral[i] = this->numeral[i - 1];
        }
        this->numeral.push_back(msd);
        tmp -= BigInt::DIGIT_BIT;
    }

    digit_t shift = tmp.numeral[0];
    size_t length = this->numeral.size();

    digit_t msb = this->numeral[length - 1] >> (BigInt::DIGIT_BIT - shift); // most significant bits
    for (size_t i = length - 1; i < length - 1; i--)
    {
        this->numeral[i] = this->numeral[i] << shift;
        this->numeral[i] |= this->numeral[i - 1] >> (BigInt::DIGIT_BIT - shift);
    }
    this->numeral[0] <<= shift;
    if (msb != 0)
    {
        this->numeral.push_back(msb);
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

    //FIXME::
    BigInt tmp(o);

    while (tmp >= BigInt::DIGIT_BIT)
    {
        size_t length = this->numeral.size();
        for (size_t i = 0; i < length - 1; i++)
        {
            this->numeral[i] = this->numeral[i + 1];
        }
        this->numeral.erase(this->numeral.end());
        if (*this == 0)
        {
            return *this;
        }
        tmp -= BigInt::DIGIT_BIT;
    }

    digit_t shift = tmp.numeral[0];
    size_t length = this->numeral.size();
    for (size_t i = 0; i < length - 1; i++)
    {
        this->numeral[i] = this->numeral[i] >> shift;
        this->numeral[i] |= this->numeral[i + 1] << (BigInt::DIGIT_BIT - shift);
    }

    this->numeral[length - 1] >>= shift;

    if (this->numeral.back() == 0)
    {
        this->numeral.erase(this->numeral.end());
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

        os << std::hex << a.numeral[length - 1];
        for (size_t i = length - 2; i < length; i--)
        {
            auto digit = a.numeral[i];
            os << std::setw(8) << std::setfill('0') << digit;
            //os << digit;
        }
        //os << '[' << std::dec << length << ']';
    }
    else
    {
        os << '0';
    }

    return os;
}

} // namespace bigint
