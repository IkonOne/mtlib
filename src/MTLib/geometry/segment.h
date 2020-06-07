#ifndef _MTLIB_GEOMETRY_SEGMENT_H_
#define _MTLIB_GEOMETRY_SEGMENT_H_

#include "MTLib/algebra/vec.h"

#include <array>
#include <cstddef>  // size_t

namespace mtlib {

template <std::size_t N, typename Scalar>
class segment {
public:
    segment() = default;
    segment(std::array<vec<N, Scalar>, 2> ends) : endpoints(ends) {}

    // accessors
    vec<N, Scalar>& at(std::size_t idx) { return endpoints[idx]; }
    const vec<N, Scalar>& at(std::size_t idx) const { return endpoints[idx]; }

    vec<N, Scalar> &operator[](std::size_t idx) { return endpoints[idx]; }
    const vec<N, Scalar>& operator[](std::size_t idx) const { return endpoints[idx]; }

    // lexicographic comparisons
    bool operator==(const vec<N, Scalar>& rhs) { return endpoints == rhs.endpoints; }
    bool operator!=(const vec<N, Scalar>& rhs) { return endpoints != rhs.endpoints; }
    bool operator<(const vec<N, Scalar>& rhs) { return endpoints < rhs.endpoints; }
    bool operator<=(const vec<N, Scalar>& rhs) { return endpoints <= rhs.endpoints; }
    bool operator>(const vec<N, Scalar>& rhs) { return endpoints > rhs.endpoints; }
    bool operator>=(const vec<N, Scalar>& rhs) { return endpoints >= rhs.endpoints; }

private:
    std::array<vec<N, Scalar>, 2> endpoints;
};

}   // namespace mtlib

#endif // _MTLIB_GEOMETRY_SEGMENT_H_