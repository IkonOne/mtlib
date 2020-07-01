#ifndef _MTLIB_ALGEBRA_COMMON_H_
#define _MTLIB_ALGEBRA_COMMON_H_

#include <cassert>
#include <cmath>

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

/**
 * Calculates a pseudo angle similar to atan2.
 *
 * Impl: https://stackoverflow.com/questions/16542042/fastest-way-to-sort-vectors-by-angle-without-actually-computing-that-angle
 * Note: Another option is Fowler Angles: https://steve.hollasch.net/cgindex/math/fowler.html
 *
 * @tparam Scalar Pseudo angle between [-2, 2).  Monotonically increasing.
 * @param x
 * @param y
 * @return
 */
template <typename Scalar>
constexpr Scalar pseudo_angle(Scalar x, Scalar y) {
    assert(std::abs(x) + std::abs(y) != (Scalar)0);
    return std::copysign(
        (Scalar)1 - x / (std::abs(x) + std::abs(y)),
        (y == (Scalar)0 ? -(Scalar)0 : y)
    );
}


}

#endif // _MTLIB_ALGEBRA_COMMON_H_