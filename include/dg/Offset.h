#ifndef DG_OFFSET_H_
#define DG_OFFSET_H_

#include <cstdint>

#ifndef NDEBUG
#include <iostream>
#endif // not NDEBUG

namespace dg {

// just a wrapper around uint64_t to
// handle Offset::UNKNOWN somehow easily
// maybe later we'll make it a range
struct Offset
{
    using type = uint64_t;

    // the value used for the unknown offset
    static const type UNKNOWN;

    static Offset getUnknown() {
        return Offset(Offset::UNKNOWN);
    }

    static Offset getZero() {
        return Offset(0);
    }

    // cast to type
    //operator type() { return offset; }

    Offset(type o = UNKNOWN) : offset(o) {}
    Offset(const Offset&) = default;

    Offset operator+(const Offset o) const
    {
        if (offset == UNKNOWN || o.offset == UNKNOWN ||
            offset >= UNKNOWN - o.offset) {
            return UNKNOWN;
        }

        return Offset(offset + o.offset);
    }

    Offset& operator+=(const Offset o)
    {
        if (offset == UNKNOWN || o.offset == UNKNOWN ||
            offset >= UNKNOWN - o.offset) {
            offset = UNKNOWN;
        } else {
            offset += o.offset;
        }

        return *this;
    }

    Offset& operator=(const Offset o)
    {
        offset = o.offset;
        return *this;
    }

    Offset operator-(const Offset& o) const
    {
        if (offset == UNKNOWN || o.offset == UNKNOWN ||
            offset < o.offset) {
            return Offset(UNKNOWN);
        }

        return Offset(offset - o.offset);
    }

    Offset& operator-(const Offset& o)
    {
        if (offset == UNKNOWN || o.offset == UNKNOWN ||
            offset < o.offset) {
            offset = UNKNOWN;
        } else {
            offset -= o.offset;
        }
        return *this;
    }

    Offset& operator~()
    {
        if (offset != UNKNOWN) {
            offset = ~offset;
        }
        return *this;
    }

    Offset operator~() const
    {
        if (offset != UNKNOWN) {
            return Offset(~offset);
        }
        return Offset::UNKNOWN;
    }

    // strict comparision (no 'maybe' comparions
    // that arises due to UNKNOWN)
    bool operator<(const Offset& o) const {
        return offset < o.offset;
    }

    bool operator>(const Offset& o) const {
        return offset > o.offset;
    }

    bool operator<=(const Offset& o) const {
        return offset <= o.offset;
    }

    bool operator>=(const Offset& o) const {
        return offset >= o.offset;
    }

    bool operator==(const Offset& o) const {
        return offset == o.offset;
    }

    bool operator!=(const Offset& o) const {
        return offset != o.offset;
    }

    bool inRange(type from, type to) const {
        return (offset >= from && offset <= to);
    }

    bool isUnknown() const { return offset == UNKNOWN; }
    bool isZero() const { return offset == 0; }

    type operator*() const { return offset; }
    const type *operator->() const { return &offset; }

#ifndef NDEBUG
    friend std::ostream& operator<<(std::ostream& os, const Offset& o) {
        if (o.isUnknown())
            os << "?";
        else
            os << o.offset;
        return os;
    }

    void dump() const {
        std::cout << *this << "\n";
    }
#endif // not NDEBUG


    type offset;
};

} // namespace dg

#include <functional>

namespace std {
template <> struct hash<dg::Offset> {
    size_t operator()(const dg::Offset& o) const { return *o; }
};
} // namespace std

#endif
