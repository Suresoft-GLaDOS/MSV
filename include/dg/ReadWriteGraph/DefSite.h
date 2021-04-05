#ifndef DG_DEF_SITE_H_
#define DG_DEF_SITE_H_

#include <set>
#include <cassert>
#include <map>
#include <list>

#include "dg/Offset.h"
#include "dg/ADT/IntervalsList.h"

namespace dg {
namespace dda {

class RWNode;

/// Take two intervals (a, a_len) and (b, b_len) where 'a' ('b', resp.) is the
// start of the interval and 'a_len' ('b_len', resp.) is the length of the
// interval and check whether their are disjunctive.
// The length can be Offset::UNKNOWN for unknown length.
// The start ('a' and 'b') must be concrete numbers.
// \return true iff intervals are disjunctive
//         false iff intervals are not disjunctive
inline bool
intervalsDisjunctive(uint64_t a, uint64_t a_len,
                     uint64_t b, uint64_t b_len) {
    assert(a != Offset::UNKNOWN && "Start of an interval is unknown");
    assert(b != Offset::UNKNOWN && "Start of an interval is unknown");
    assert(a_len > 0 && "Interval of lenght 0 given");
    assert(b_len > 0 && "Interval of lenght 0 given");

    if (a_len == Offset::UNKNOWN) {
        if (b_len == Offset::UNKNOWN) {
            return false;
        } else {
            // b_len is concrete and a_len is unknown
            // use less or equal, because we are starting
            // from 0 and the bytes are distinct (e.g. 4th byte
            // is on offset 3)
            return (a <= b) ? false : b_len <= a - b;
        }
    } else if (b_len == Offset::UNKNOWN) {
        return (a <= b) ? a_len <= b - a : false;
    }

    // the lenghts and starts are both concrete
    return ((a <= b) ? (a_len <= b - a) : (b_len <= a - b));
}

///
// Take two intervals (a1, a2) and (b1, b2)
// (over non-negative whole numbers) and check
//  whether they overlap (not sharply, i.e.
//  if a2 == b1, then itervals already overlap)
inline bool
intervalsOverlap(uint64_t a1, uint64_t a2,
                 uint64_t b1, uint64_t b2) {
    return !intervalsDisjunctive(a1, a2, b1, b2);
}

template <typename NodeT>
struct GenericDefSite {
    using NodeTy = NodeT;

    GenericDefSite(NodeT *t,
                   const Offset& o = Offset::UNKNOWN,
                   const Offset& l = Offset::UNKNOWN)
        : target(t), offset(o), len(l) {
        assert((o.isUnknown() || l.isUnknown() ||
               *o + *l > 0) && "Invalid offset and length given");
    }

    bool operator<(const GenericDefSite& oth) const {
        return target == oth.target ?
                (offset == oth.offset ? len < oth.len : offset < oth.offset)
                : target < oth.target;
    }

    bool operator==(const GenericDefSite& oth) const {
        return target == oth.target && offset == oth.offset && len == oth.len;
    }

    Offset end() const {
        // if the offset is unknown, stretch the interval over all possible bytes
        if (offset.isUnknown()) {
            return Offset::UNKNOWN;
        } else {
            return offset + (len - 1);
        }
    }

    std::pair<Offset, Offset> getInterval() const {
        // if the offset is unknown, stretch the interval over all possible bytes
        if (offset.isUnknown()) {
            return {0, Offset::UNKNOWN};
        } else {
            return {offset, offset + (len - 1)};
        }
    }

    // what memory this node defines
    NodeT *target;
    // on what offset
    Offset offset;
    // how many bytes
    Offset len;
};

// for compatibility until we need to change it
using DefSite = GenericDefSite<RWNode>;

extern RWNode *UNKNOWN_MEMORY;

// FIXME: change this std::set to std::map (target->offsets)
class DefSiteSet : public std::set<DefSite> {
public:
    DefSiteSet intersect(const DefSiteSet& rhs) const {
        std::map<DefSite::NodeTy *, IntervalsList> lhssites;
        std::map<DefSite::NodeTy *, IntervalsList> rhssites;

        for (auto& ds : *this) {
            lhssites[ds.target].add(ds.getInterval());
        }
        for (auto& ds : rhs) {
            rhssites[ds.target].add(ds.getInterval());
        }

        DefSiteSet retval;

        for (auto& lit : lhssites) {
            auto rit = rhssites.find(lit.first);
            if (rit != rhssites.end()) {
                for (const auto& I : lit.second.intersectWith(rit->second)) {
                    retval.emplace(lit.first, I.start, I.length());
                }
            }
        }

        return retval;
    }

    template <typename Container>
    void add(const Container& C) {
        for (auto& e : C) {
            insert(e);
        }
    }
};

// FIXME: get rid of this using
using DefSiteSetT = DefSiteSet;

// wrapper around std::set<> with few
// improvements that will be handy in our set-up
/*
class RWNodesSet {
    using ContainerTy = std::set<RWNode *>;

    ContainerTy nodes;
    bool is_unknown;

public:
    RWNodesSet() : is_unknown(false) {}

    // the set contains unknown mem. location
    void makeUnknown() {
        nodes.clear();
        nodes.insert(UNKNOWN_MEMORY);
        is_unknown = true;
    }

    bool insert(RWNode *n) {
        if (is_unknown)
            return false;

        if (n == UNKNOWN_MEMORY) {
            makeUnknown();
            return true;
        } else
            return nodes.insert(n).second;
    }

    size_t count(RWNode *n) const { return nodes.count(n); }
    size_t size() const { return nodes.size(); }

    bool isUnknown() const { return is_unknown; }

    void clear() {
        nodes.clear();
        is_unknown = false;
    }

    ContainerTy::iterator begin() { return nodes.begin(); }
    ContainerTy::iterator end() { return nodes.end(); }
    ContainerTy::const_iterator begin() const { return nodes.begin(); }
    ContainerTy::const_iterator end() const { return nodes.end(); }

    ContainerTy& getNodes() {
        return nodes;
    };

};
*/

} // namespace dda
} // namespace dg

#endif
