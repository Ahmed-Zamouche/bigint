#include <iostream>
#include <vector>
#include <cstdint>
#include <cstddef>
#include <climits>

namespace bigint
{

typedef uint32_t digit_t;

typedef uint64_t uddigit_t;
typedef int64_t ddigit_t;

class BigInt
{
private:
    int sign;
    static const int SIGN_POS;
    static const int SIGN_NEG;

    static const uddigit_t DIGIT_MAX;
    static const uddigit_t DDIGIT_MAX;
    static const uddigit_t BASE;

    static const size_t DIGIT_BIT;

    static const size_t KARATSUBA_THRESHOLD;

    static std::pair<digit_t, digit_t> add(digit_t a, digit_t b, digit_t d);
    static std::pair<digit_t, digit_t> sub(digit_t a, digit_t b, digit_t d);

    std::vector<digit_t> numeral;
    static ddigit_t cmp(const BigInt &l, const BigInt &r);
    static BigInt complement(const BigInt &l);

    BigInt &baseMul(const BigInt &o);
    BigInt &karatsubaMul(const BigInt &o);
    BigInt &baseDiv(const BigInt &o);
    BigInt &baseReduce(const BigInt &o);

    inline BigInt &trim()
    {
        while (this->numeral.back() == 0)
        {
            this->numeral.pop_back();
        }
        return *this;
    }
    inline BigInt &pad(size_t n)
    {
        this->numeral.insert(this->numeral.end(), n, 0);
        return *this;
    }

    static inline BigInt trim(const BigInt &o)
    {
        BigInt tmp(o);
        return tmp.trim();
    }
    static inline BigInt pad(const BigInt &o, size_t n)
    {
        BigInt tmp(o);
        return tmp.pad(n);
    }

public:
    BigInt();
    BigInt(int v) : BigInt((ddigit_t)v){};
    BigInt(ddigit_t v);
    BigInt(uddigit_t v);
    BigInt(std::string s);
    BigInt(std::string s, int base);
    BigInt(const BigInt &o);

    static BigInt baseMul(const BigInt &l, const BigInt &r);
    static BigInt karatsubaMul(BigInt &l, BigInt &r);
    static std::pair<BigInt, BigInt> baseDiv(const BigInt &l, const BigInt &r);
    static std::pair<BigInt, BigInt> baseUnsignedDiv(BigInt &r, BigInt &d);

    BigInt &operator++();
    BigInt &operator--();
    BigInt operator++(int)
    {
        BigInt tmp(*this);
        operator++();
        return tmp;
    }
    BigInt operator--(int)
    {
        BigInt tmp(*this);
        operator--();
        return tmp;
    }
    inline BigInt operator+()
    {
        BigInt tmp(*this);
        return tmp;
    }
    inline BigInt operator-()
    {
        BigInt tmp(*this);
        tmp.sign = -this->sign;
        return tmp;
    }
    inline BigInt operator~()
    {
        BigInt tmp = ((*this)++);
        tmp.sign = -tmp.sign;
        return tmp;
    }
    inline bool operator!()
    {
        return this->numeral.size() == 0;
    }

    inline BigInt &abs()
    {
        this->sign = BigInt::SIGN_POS;
        return *this;
    }

    BigInt &operator+=(const BigInt &o);
    BigInt &operator-=(const BigInt &o);
    BigInt &operator*=(const BigInt &o);
    BigInt &operator/=(const BigInt &o);
    BigInt &operator%=(const BigInt &o);

    BigInt &operator&=(const BigInt &o);
    BigInt &operator|=(const BigInt &o);
    BigInt &operator^=(const BigInt &o);

    BigInt &operator>>=(const BigInt &o);
    BigInt &operator<<=(const BigInt &o);

    inline BigInt &operator+=(const ddigit_t o)
    {
        *this += BigInt(o);
        return *this;
    }
    inline BigInt &operator-=(const ddigit_t o)
    {
        *this -= BigInt(o);
        return *this;
    }
    inline BigInt &operator*=(const ddigit_t o)
    {
        *this *= BigInt(o);
        return *this;
    }
    inline BigInt &operator/=(const ddigit_t o)
    {
        *this /= BigInt(o);
        return *this;
    }
    inline BigInt &operator%=(const ddigit_t o)
    {
        *this %= BigInt(o);
        return *this;
    }

    inline BigInt &operator&=(const ddigit_t o)
    {
        *this &= BigInt(o);
        return *this;
    }
    inline BigInt &operator|=(const ddigit_t o)
    {
        *this |= BigInt(o);
        return *this;
    }
    inline BigInt &operator^=(const ddigit_t o)
    {
        *this ^= BigInt(o);
        return *this;
    }

    inline BigInt &operator>>=(const ddigit_t o)
    {
        *this >>= BigInt(o);
        return *this;
    }
    inline BigInt &operator<<=(const ddigit_t o)
    {
        *this <<= BigInt(o);
        return *this;
    }

    friend inline BigInt operator+(const BigInt &l, const ddigit_t r)
    {
        return l + BigInt(r);
    }
    friend inline BigInt operator+(const ddigit_t l, const BigInt &r)
    {
        return BigInt(r) + l;
    }
    friend inline BigInt operator+(const BigInt &l, const BigInt &r)
    {
        BigInt tmp(l);
        tmp += r;
        return tmp;
    }
    friend inline BigInt operator-(const BigInt &l, const ddigit_t r)
    {
        return l - BigInt(r);
    }
    friend inline BigInt operator-(const ddigit_t l, const BigInt &r)
    {
        return BigInt(l) - r;
    }
    friend inline BigInt operator-(const BigInt &l, const BigInt &r)
    {
        BigInt tmp(l);
        tmp -= r;
        return tmp;
    }
    friend inline BigInt operator*(const BigInt &l, const ddigit_t r)
    {
        return l * BigInt(r);
    }
    friend inline BigInt operator*(const ddigit_t l, const BigInt &r)
    {
        return BigInt(l) * r;
    }
    friend inline BigInt operator*(const BigInt &l, const BigInt &r)
    {
        BigInt tmp(l);
        tmp *= r;
        return tmp;
    }
    friend inline BigInt operator/(const BigInt &l, const ddigit_t r)
    {
        return l / BigInt(r);
    }
    friend inline BigInt operator/(const ddigit_t l, const BigInt &r)
    {
        return BigInt(l) / r;
    }
    friend inline BigInt operator/(const BigInt &l, const BigInt &r)
    {
        BigInt tmp(l);
        tmp /= r;
        return tmp;
    }
    friend inline BigInt operator%(const BigInt &l, const ddigit_t r)
    {
        return l % BigInt(r);
    }
    friend inline BigInt operator%(const ddigit_t l, const BigInt &r)
    {
        return BigInt(l) % r;
    }
    friend inline BigInt operator%(const BigInt &l, const BigInt &r)
    {
        BigInt tmp(l);
        tmp %= r;
        return tmp;
    }
    friend inline BigInt operator&(const BigInt &l, const ddigit_t r)
    {
        return l & BigInt(r);
    }
    friend inline BigInt operator&(const ddigit_t l, const BigInt &r)
    {
        return BigInt(l) & r;
    }
    friend inline BigInt operator&(const BigInt &l, const BigInt &r)
    {
        BigInt tmp(l);
        tmp &= r;
        return tmp;
    }
    friend inline BigInt operator|(const BigInt &l, const ddigit_t r)
    {
        return l | BigInt(r);
    }
    friend inline BigInt operator|(const ddigit_t l, const BigInt &r)
    {
        return BigInt(l) | r;
    }
    friend inline BigInt operator|(const BigInt &l, const BigInt &r)
    {
        BigInt tmp(l);
        tmp |= r;
        return tmp;
    }
    friend inline BigInt operator^(const BigInt &l, const ddigit_t r)
    {
        return l ^ BigInt(r);
    }
    friend inline BigInt operator^(const ddigit_t l, const BigInt &r)
    {
        return BigInt(l) ^ r;
    }
    friend inline BigInt operator^(const BigInt &l, const BigInt &r)
    {
        BigInt tmp(l);
        tmp ^= r;
        return tmp;
    }
    friend inline BigInt operator>>(const BigInt &l, const ddigit_t r)
    {
        return l >> BigInt(r);
    }
    friend inline BigInt operator>>(const ddigit_t l, const BigInt &r)
    {
        return BigInt(l) >> r;
    }
    friend inline BigInt operator>>(const BigInt &l, const BigInt &r)
    {
        BigInt tmp(l);
        tmp >>= r;
        return tmp;
    }
    friend inline BigInt operator<<(const BigInt &l, const ddigit_t r)
    {
        return l << BigInt(r);
    }
    friend inline BigInt operator<<(const ddigit_t l, const BigInt &r)
    {
        return BigInt(l) << r;
    }
    friend inline BigInt operator<<(const BigInt &l, const BigInt &r)
    {
        BigInt tmp(l);
        tmp <<= r;
        return tmp;
    }

    friend inline bool operator==(const BigInt &l, const ddigit_t r) { return BigInt::cmp(l, BigInt(r)) == 0; }
    friend inline bool operator==(const ddigit_t l, const BigInt &r) { return BigInt::cmp(BigInt(l), r) == 0; }
    friend inline bool operator==(const BigInt &l, const BigInt &r) { return BigInt::cmp(l, r) == 0; }

    friend inline bool operator!=(const BigInt &l, const ddigit_t r) { return BigInt::cmp(l, BigInt(r)) != 0; }
    friend inline bool operator!=(const ddigit_t l, const BigInt &r) { return BigInt::cmp(BigInt(l), r) != 0; }
    friend inline bool operator!=(const BigInt &l, const BigInt &r) { return BigInt::cmp(l, r) != 0; }

    friend inline bool operator<(const BigInt &l, const ddigit_t r) { return BigInt::cmp(l, BigInt(r)) < 0; }
    friend inline bool operator<(const ddigit_t l, const BigInt &r) { return BigInt::cmp(BigInt(l), r) < 0; }
    friend inline bool operator<(const BigInt &l, const BigInt &r) { return BigInt::cmp(l, r) < 0; }

    friend inline bool operator>(const BigInt &l, const ddigit_t r) { return BigInt::cmp(l, BigInt(r)) > 0; }
    friend inline bool operator>(const ddigit_t l, const BigInt &r) { return BigInt::cmp(BigInt(l), r) > 0; }
    friend inline bool operator>(const BigInt &l, const BigInt &r) { return BigInt::cmp(l, r) > 0; }

    friend inline bool operator<=(const BigInt &l, const ddigit_t r) { return BigInt::cmp(l, BigInt(r)) <= 0; }
    friend inline bool operator<=(const ddigit_t l, const BigInt &r) { return BigInt::cmp(BigInt(l), r) <= 0; }
    friend inline bool operator<=(const BigInt &l, const BigInt &r) { return BigInt::cmp(l, r) <= 0; }

    friend inline bool operator>=(const BigInt &l, const ddigit_t r) { return BigInt::cmp(l, BigInt(r)) >= 0; }
    friend inline bool operator>=(const ddigit_t l, const BigInt &r) { return BigInt::cmp(BigInt(l), r) >= 0; }
    friend inline bool operator>=(const BigInt &l, const BigInt &r) { return BigInt::cmp(l, r) >= 0; }

    ~BigInt();
    friend std::ostream &operator<<(std::ostream &os, const BigInt &dt);
};

inline BigInt abs(const BigInt &o) { return (BigInt(o)).abs(); }

} // namespace bigint
