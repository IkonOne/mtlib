#ifndef _MTLIB_ALGEBRA_LINALG_H_
#define _MTLIB_ALGEBRA_LINALG_H_

#include "MTLib/algebra/vec.h"

namespace mtlib {

template <typename Scalar>
inline Scalar orientation(const vec2<Scalar>& v1, const vec2<Scalar>& v2, const vec2<Scalar>& v3) {
    return dot_perp(v2 - v1, v3 - v1);
}

template <typename InputIt, typename Scalar>
inline Scalar orientation(const InputIt& it1, const InputIt& it2, const InputIt& it3) {
    return orientation(*it1, *it2, *it3);
}

template <typename Scalar>
inline bool is_ccw(const vec2<Scalar>& v1, const vec2<Scalar>& v2, const vec2<Scalar>& v3) {
    return orientation(v1, v2, v3) > (Scalar)0;
}

template <typename InputIt>
inline bool is_ccw(const InputIt& it1, const InputIt& it2, const InputIt& it3) {
    return is_ccw(*it1, *it2, *it3);
}

template <typename Scalar>
inline bool is_cw(const vec2<Scalar>& v1, const vec2<Scalar>& v2, const vec2<Scalar>& v3) {
    return orientation(v1, v2, v3) < (Scalar)0;
}

template <typename InputIt>
inline bool is_cw(const InputIt& it1, const InputIt& it2, const InputIt& it3) {
    return is_cw(*it1, *it2, *it3);
}

template <typename Scalar>
inline bool is_colinear(const vec2<Scalar>& v1, const vec2<Scalar>& v2, const vec2<Scalar>& v3) {
    return orientation(v1, v2, v3) == (Scalar)0;
}

template <typename InputIt>
inline bool is_colinear(const InputIt& it1, const InputIt& it2, const InputIt& it3) {
    return is_colinear(*it1, *it2, *it3);
}

}   // namespace mtlib

#endif // _MTLIB_ALGEBRA_LINALG_H_