#ifndef _MTLIB_ALGEBRA_LINALG_H_
#define _MTLIB_ALGEBRA_LINALG_H_

#include "MTLib/algebra/vec.h"

namespace mtlib {

template <typename Scalar>
inline int orientation(const vec2<Scalar>& v1, const vec2<Scalar>& v2, const vec2<Scalar>& v3) {
    auto orient = dot_perp(v2 - v1, v3 - v1);
    if (orient == (Scalar)0) return 0;
    return orient < (Scalar)0 ? -1 : 1;
}

template <typename Scalar>
inline int is_ccw(const vec2<Scalar>& v1, const vec2<Scalar>& v2, const vec2<Scalar>& v3) {
    return dot_perp(v2 - v1, v3 - v1) > (Scalar)0;
}

template <typename Scalar>
inline int is_cw(const vec2<Scalar>& v1, const vec2<Scalar>& v2, const vec2<Scalar>& v3) {
    return dot_perp(v2 - v1, v3 - v1) < (Scalar)0;
}

}   // namespace mtlib

#endif // _MTLIB_ALGEBRA_LINALG_H_