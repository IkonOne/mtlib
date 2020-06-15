#ifndef _MTLIB_INTERSECT_SEGMENT_VEC_2D_H_
#define _MTLIB_INTERSECT_SEGMENT_VEC_2D_H_

#include "MTLib/algebra/linalg.h"
#include "MTLib/algebra/vec.h"
#include "MTLib/geometry/segment.h"

#include <cmath>

namespace mtlib {

template <typename Scalar>
constexpr bool overlap_segment_vec_2D(const segment2<Scalar>& seg, const vec2<Scalar>& v) {
    return
        is_colinear(seg[0], seg[1], v) &&
        min_on_dim(seg, 0) <= v[0] && v[0] <= max_on_dim(seg, 0) &&
        min_on_dim(seg, 1) <= v[1] && v[1] <= max_on_dim(seg, 1);
}

}

#endif // _MTLIB_INTERSECT_SEGMENT_VEC_2D_H_