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
    segment(const vec<N, Scalar>& v1, const vec<N, Scalar>& v2)
        : endpoints({v1, v2})
    {}

    // accessors
    vec<N, Scalar>& at(std::size_t idx) { return endpoints[idx]; }
    const vec<N, Scalar>& at(std::size_t idx) const { return endpoints[idx]; }

    vec<N, Scalar> &operator[](std::size_t idx) { return endpoints[idx]; }
    const vec<N, Scalar>& operator[](std::size_t idx) const { return endpoints[idx]; }

    // lexicographic comparisons
    bool operator==(const vec<N, Scalar>& rhs) { return endpoints == rhs.endpoints; }
    bool operator!=(const vec<N, Scalar>& rhs) { return endpoints != rhs.endpoints; }
    bool operator< (const vec<N, Scalar>& rhs) { return endpoints <  rhs.endpoints; }
    bool operator<=(const vec<N, Scalar>& rhs) { return endpoints <= rhs.endpoints; }
    bool operator> (const vec<N, Scalar>& rhs) { return endpoints >  rhs.endpoints; }
    bool operator>=(const vec<N, Scalar>& rhs) { return endpoints >= rhs.endpoints; }

private:
    std::array<vec<N, Scalar>, 2> endpoints;
};

// common aliases
template <typename Scalar>
using segment2 = segment<2, Scalar>;
using segment2f = segment2<float>;
using segment2d = segment2<double>;
using segment2l = segment2<long double>;

template <typename Scalar>
using segment3 = segment<3, Scalar>;
using segment3f = segment3<float>;
using segment3d = segment3<double>;
using segment3l = segment3<long double>;

template <typename Scalar>
using segment4 = segment<4, Scalar>;
using segment4f = segment4<float>;
using segment4d = segment4<double>;
using segment4l = segment4<long double>;


}   // namespace mtlib

#endif // _MTLIB_GEOMETRY_SEGMENT_H_