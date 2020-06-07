#ifndef _MTLIB_ALGEBRA_COMMON_H_
#define _MTLIB_ALGEBRA_COMMON_H_

namespace mtlib {

template <typename Scalar>
constexpr Scalar lerp(Scalar a, Scalar b, Scalar t) noexcept {
    return a + ((b - a) * t);
}

}

#endif // _MTLIB_ALGEBRA_COMMON_H_