#include <cstdint>
#include <cstddef>
#include <iostream>
#include <vector>
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
    static const int SIGN_POS = +1;
    static const int SIGN_NEG = -1;

    static const uddigit_t DIGIT_MAX = (uddigit_t)((digit_t)-1);
    static const uddigit_t DDIGIT_MAX = DIGIT_MAX + 1;

    static const size_t DIGIT_BIT = sizeof(digit_t) * CHAR_BIT;

    std::vector<digit_t> numeral;
    static ddigit_t cmp(const BigInt &l, const BigInt &r);

  public:
    BigInt();
    BigInt(ddigit_t v);
    BigInt(std::string s);
    BigInt(std::string s, int base);
    BigInt(const BigInt &o);

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
    BigInt operator+()
    {
        BigInt tmp(*this);
        return tmp;
    }
    BigInt operator-()
    {
        BigInt tmp(*this);
        tmp.sign = -this->sign;
        return tmp;
    }
    BigInt operator~()
    {
        BigInt tmp = (*this + BigInt(1));
        tmp.sign = -tmp.sign;
        return tmp;
    }
    bool operator!()
    {
        return this->numeral.size() == 0;
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

} // namespace bigint
