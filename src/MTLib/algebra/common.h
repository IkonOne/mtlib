#ifndef _MTLIB_ALGEBRA_COMMON_H_
#define _MTLIB_ALGEBRA_COMMON_H_

#include <cassert>

namespace mtlib {

template <typename Scalar>
constexpr Scalar lerp(Scalar a, Scalar b, Scalar t) noexcept {
    return a + ((b - a) * t);
}

template <typename Scalar>
constexpr Scalar inv_lerp(Scalar start, Scalar end, Scalar in) {
    assert(start != end);
    return (in - start) / (end - start);
}

}

#endif // _MTLIB_ALGEBRA_COMMON_H_