#ifndef _MTLIB_INTERSECT_SEGMENT_SEGMENT_2D_H_
#define _MTLIB_INTERSECT_SEGMENT_SEGMENT_2D_H_

#include "MTLib/algebra/vec.h"
#include "MTLib/geometry/segment.h"
#include "MTLib/intersection/result_segment_segment.h"

#include <array>
#include <cmath>
#include <utility>

namespace mtlib {

// FIXME: Should this be in separate header?
template <typename Scalar>
bool overlap_segment_segment_2D(const segment2<Scalar>& seg1, const segment2<Scalar>& seg2) {
    auto o1 = signed_area_2D(seg1[0], seg1[1], seg2[0]);
    auto o2 = signed_area_2D(seg1[0], seg1[1], seg2[1]);
    auto o12 = o1 * o2;

    if (o12 <= (Scalar)0) {
        auto o3 = signed_area_2D(seg2[0], seg2[1], seg1[0]);
        auto o4 = o1 + o2 - o3; // signed_area_2D(seg2[0], seg2[1], seg1[0]);
        auto o34 = o3 * o4;

        // if o1 and o2 are opposing and o3 and o4 are opposing
        if (o12 < (Scalar)0 && o34 < (Scalar)0)
            return true;

        // in the case that one or both of the endpoints of a segment are colinear with the other segment
        // we test if the endpoint is within the bounds of the other segment
        auto on_segment = [](const segment2<Scalar>& seg, const vec2<Scalar>& v) -> bool {
            return min_on_dim(seg, 0) <= v[0] && v[0] <= max_on_dim(seg, 0) &&
                min_on_dim(seg, 1) <= v[1] && v[1] <= max_on_dim(seg, 1);
        };

        return (o1 == (Scalar)0 && on_segment(seg1, seg2[0])) ||
            (o2 == (Scalar)0 && on_segment(seg1, seg2[1])) ||
            (o3 == (Scalar)0 && on_segment(seg2, seg1[0])) ||
            (o4 == (Scalar)0 && on_segment(seg2, seg1[1]));
    }

    return false;
}

}   // namespace mtlib

#endif // _MTLIB_INTERSECT_SEGMENT_SEGMENT_2D_H_