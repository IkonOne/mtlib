#ifndef _MTLIB_OVERLAP_CONVEX_POINT_2D_H_
#define _MTLIB_OVERLAP_CONVEX_POINT_2D_H_

#include "MTLib/algebra/vec.h"
#include "MTLib/comp_geo/is_convex_2d.h"

#include <cassert>
#include <iterator>
#include <vector>

namespace mtlib {

template <typename RandomIt, typename Scalar>
bool overlap_convex_point_2d(const RandomIt& first, const RandomIt& last, const vec2<Scalar> &p) {
    assert(std::distance(first, last) >= 3);
    assert(is_convex_2d(first, last));

    int n = std::distance(first, last);

    auto ccw_or_colinear = [](const vec2<Scalar>& v1, const vec2<Scalar>& v2, const vec2<Scalar>& v3) {
        return signed_area_2D(v1, v2, v3) >= 0;
    };

    if (n == 3) {
        return (
            ccw_or_colinear(first[0], first[1], p) &&
            ccw_or_colinear(first[1], first[2], p) &&
            ccw_or_colinear(first[2], first[0], p)
        );
    }

    int mid = n / 2;
    auto p1 = first[0];
    auto p2 = first[mid];

    std::vector<vec2<Scalar>> sub_hull;
    if (dot_perp(p2 - p1, p - p1) > 0)
        copy(first, first + mid, sub_hull.begin());
    else
        copy(first + (n > 4 ? mid : 1), last, sub_hull.begin());

    return overlap_convex_point_2d(sub_hull.begin(), sub_hull.end(), p);
}

} // namespace mtlib

#endif // _MTLIB_OVERLAP_CONVEX_POINT_2D_H_