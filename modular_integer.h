#ifndef __MODINT__
#define __MODINT__

#include <QDataStream>

template <typename T, T mod> class modint /* modular integer */
{
private:
    T value;

    void add(T val)
    {
        /* This function needs to be implemented with care!
         * All operations must fit into T even if mod = T max!
         * Consequently, all partial results must be less then or equal to mod!
         * Thus, properly placed brackets are necessary!
         * Explanation:
         *     Do val+value if val+value < mod
         *     Do val+value-mod if mod <= val+value
         * We should decide whether val+value < mod, but this
         * unequality must be rearranged to ensure that both
         * sides are fit into the valid value range:
         *     val < mod-value
         * Verify that the result is in the valid value range:
         *     If val < mod-value, then val+value < mod
         *     If val >= mod-value, then val-(mod-value) >= 0,
         *     after rearrangement: 0 <= val-(mod-value) <= val <= mod
         */
        value = val < mod-value ? val+value : val-(mod-value);
    }
    void sub(T val)
    {
        /* This function needs to be implemented with care!
         * All operations must fit into T even if mod = T max!
         * Consequently, all partial results must be less then or equal to mod!
         * Thus, properly placed brackets are necessary!
         * Explanation:
         *     Do value-val if val <= value
         *     Do value-val+mod if value < val
         * Verify that the result is in the valid value range:
         *     If val <= value, then 0 <= value-val
         *     If val >  value, then mod > mod-val+value,
         *     after rearrangement: 0 < mod-val < mod-value <= mod
         */
        value = val <= value ? value-val : (mod-val)+value;
    }
public:
    T get() const
    {
        return value;
    }
    modint(T val=0) { value=val%mod; }
    modint<T,mod>& operator =(T val) { value=val%mod; return *this; }
    operator T () const { return value; }

    modint<T,mod>& operator  =(const modint<T,mod>& other) { value=other.value; return *this; }
    modint<T,mod>& operator +=(const modint<T,mod>& other) { add(other.value);  return *this; }
    modint<T,mod>& operator -=(const modint<T,mod>& other) { sub(other.value);  return *this; }

    // pre-increment
    modint<T,mod>& operator ++() { add(1); return *this; }
    modint<T,mod>& operator --() { sub(1); return *this; }

    // post-increment
    modint<T,mod> operator++(int)
    {
        modint<T,mod> tmp = *this;
        operator ++();
        return tmp;
    }
    modint<T,mod> operator--(int)
    {
        modint<T,mod> tmp = *this;
        operator --();
        return tmp;
    }
};

template <typename T, T mod>
modint<T,mod> operator +(modint<T,mod> lhs, const modint<T,mod>& rhs) {
    return lhs += rhs;
}
template <typename T, T mod>
modint<T,mod> operator +(modint<T,mod> lhs, const T& rhs) {
    return lhs + modint<T,mod>(rhs);
}
template <typename T, T mod>
modint<T,mod> operator +(const T& lhs, const modint<T,mod>& rhs) {
    return modint<T,mod>(lhs) + rhs;
}
template <typename T, T mod>
modint<T,mod> operator -(modint<T,mod> lhs, const modint<T,mod>& rhs) {
    return lhs -= rhs;
}
template <typename T, T mod>
modint<T,mod> operator -(modint<T,mod> lhs, const T& rhs) {
    return lhs - modint<T,mod>(rhs);
}
template <typename T, T mod>
modint<T,mod> operator -(const T& lhs, const modint<T,mod>& rhs) {
    return modint<T,mod>(lhs) - rhs;
}

template <typename T, T mod>
bool operator ==(const modint<T,mod>& lhs, const modint<T,mod>& rhs) {
    return lhs.get() == rhs.get();
}
template <typename T, T mod>
bool operator !=(const modint<T,mod>& lhs, const modint<T,mod>& rhs) {
    return !(lhs == rhs);
}
template <typename T, T mod>
bool operator  <(const modint<T,mod>& lhs, const modint<T,mod>& rhs) {
    return lhs.get() < rhs.get();
}
template <typename T, T mod>
bool operator  >(const modint<T,mod>& lhs, const modint<T,mod>& rhs) {
    return rhs < lhs;
}
template <typename T, T mod>
bool operator <=(const modint<T,mod>& lhs, const modint<T,mod>& rhs) {
    return !(lhs > rhs);
}
template <typename T, T mod>
bool operator >=(const modint<T,mod>& lhs, const modint<T,mod>& rhs) {
    return !(lhs < rhs);
}

template <typename T, T mod>
QDataStream& operator <<(QDataStream& out, const modint<T,mod>& mi) {
  return out << mi.get();
}

template <typename T, T mod>
QDataStream& operator >>(QDataStream& in, modint<T,mod>& mi)
{
    T val;
    in >> val;
    mi = val;
    return in;
}

#endif /* __MODINT__ */
