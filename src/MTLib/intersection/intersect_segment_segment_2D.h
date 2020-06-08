#ifndef _MTLIB_INTERSECT_SEGMENT_SEGMENT_2D_H_
#define _MTLIB_INTERSECT_SEGMENT_SEGMENT_2D_H_

#include "MTLib/geometry/segment.h"
#include "MTLib/algebra/vec.h"

#include <array>
#include <cmath>
#include <utility>

namespace mtlib {

template <typename Scalar>
bool overlap_segment_segment_2D(const segment2<Scalar>& seg1, const segment2<Scalar>& seg2) {
    auto o1 = orientation(seg1[0], seg1[1], seg2[0]);
    auto o2 = orientation(seg1[0], seg1[1], seg2[1]);
    auto o3 = orientation(seg2[0], seg2[1], seg1[0]);
    auto o4 = o1 + o2 - o3; // orientation(seg2[0], seg2[1], seg1[0]);

    // if o1 and o2 are opposing and o3 and o4 are opposing
    if (    (o1 < (Scalar)0 && (Scalar)0 < o2 || o2 < (Scalar)0 && (Scalar)0 < o1) &&
            (o3 < (Scalar)0 && (Scalar)0 < o4 || o4 < (Scalar)0 && (Scalar)0 < o3)  )
        return true;

    // in the case that one or both of the endpoints of a segment are colinear with the other segment
    // we test if the endpoint is within the bounds of the other segment
    auto on_segment = [](const vec2<Scalar>& v1, const vec2<Scalar>& v2, const vec2<Scalar>& v3) {
        return std::min(v1[0], v2[0]) <= v3[0] && v3[0] <= std::max(v1[0], v2[0]) &&
            std::min(v1[1], v2[1]) <= v3[1] && v3[1] <= std::max(v1[1], v2[1]);
    };

    if (o1 == (Scalar)0 && on_segment(seg1[0], seg1[1], seg2[0])) return true;
    if (o2 == (Scalar)0 && on_segment(seg1[0], seg1[1], seg2[1])) return true;
    if (o3 == (Scalar)0 && on_segment(seg2[0], seg2[1], seg1[0])) return true;
    if (o4 == (Scalar)0 && on_segment(seg2[0], seg2[1], seg1[1])) return true;

    return false;
}

}   // namespace mtlib

#endif // _MTLIB_INTERSECT_SEGMENT_SEGMENT_2D_H_