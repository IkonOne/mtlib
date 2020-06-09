#ifndef _MTLIB_INTERSECTION_RESULT_SEGMENT_SEGMENT_H_
#define _MTLIB_INTERSECTION_RESULT_SEGMENT_SEGMENT_H_

#include "MTLib/algebra/vec.h"
#include "MTLib/geometry/segment.h"

#include <cstddef> // size_t

namespace mtlib {
namespace intersection {

template <std::size_t N, typename Scalar>
struct result_segment_segment {
    constexpr result_segment_segment() = default;

    constexpr result_segment_segment(const segment<N, Scalar>& seg1, const segment<N, Scalar>& seg2)
        : segment1(seg1), segment2(seg2)
    {}

    vec<N, Scalar> point;
    segment<N, Scalar> segment1;
    segment<N, Scalar> segment2;
    Scalar t1;
    Scalar t2;
};

}   // namespace intersection
}   // namespace mtlib

#endif // _MTLIB_INTERSECTION_RESULT_SEGMENT_SEGMENT_H_