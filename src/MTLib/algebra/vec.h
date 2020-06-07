#ifndef _MTLIB_ALGEBRA_VECTOR_H_
#define _MTLIB_ALGEBRA_VECTOR_H_

#include <array>
#include <cmath>
#include <cstddef>  // size_t
#include <functional>   // hash
#include <type_traits>  // enable_if
#include <ratio>

namespace mtlib {

template <std::size_t N, typename Scalar>
class vec {

template <std::size_t LHS, std::size_t RHS>
struct dim_size_equal {
    enum { value = std::ratio_equal<std::ratio<LHS>, std::ratio<RHS>>::value };
};

public:
    vec() = default;

    vec(std::array<Scalar, N> l)
        : vals(l)
    {}

    // vec2 constructor
    template <std::size_t Dim = N>
    vec(const Scalar& s1, const Scalar& s2,
        typename std::enable_if< dim_size_equal<Dim, 2>::value >::type* = 0
    )
        : vals({ s1, s2 })
    { }

    // vec3 constructor
    template <std::size_t Dim = N>
    vec(const Scalar& s1, const Scalar& s2, const Scalar& s3,
        typename std::enable_if< dim_size_equal<Dim, 3>::value >::type* = 0
    )
        : vals({ s1, s2, s3 })
    { }

    // vec4 constructor
    template <std::size_t Dim = N>
    vec(const Scalar& s1, const Scalar& s2, const Scalar& s3, const Scalar& s4,
        typename std::enable_if< dim_size_equal<Dim, 4>::value >::type* = 0
    )
        : vals({ s1, s2, s3, s4 })
    { }

    // accessors
    inline Scalar& at(std::size_t idx) { return vals.at(idx); }
    inline const Scalar& at(std::size_t idx) const { return vals.at(idx); }

    inline Scalar& operator[](std::size_t idx) { return vals[idx]; }
    inline const Scalar& operator[](std::size_t idx) const { return vals[idx]; }

    // lexicographic comparisons
    inline bool operator==(const vec& rhs) { return vals == rhs.vals; }
    inline bool operator!=(const vec& rhs) { return vals != rhs.vals; }
    inline bool operator<(const vec& rhs) { return vals < rhs.vals; }
    inline bool operator<=(const vec& rhs) { return vals <= rhs.vals; }
    inline bool operator>(const vec& rhs) { return vals > rhs.vals; }
    inline bool operator>=(const vec& rhs) { return vals >= rhs.vals; }

private:
    std::array<Scalar, N> vals;
};

// common specializations
template <typename Scalar>
using vec2 = vec<2, Scalar>;
using vec2f = vec2<float>;
using vec2d = vec2<double>;
using vec2l = vec2<long double>;

template <typename Scalar>
using vec3 = vec<3, Scalar>;
using vec3f = vec3<float>;
using vec3d = vec3<double>;
using vec3l = vec3<long double>;

template <typename Scalar>
using vec4 = vec<4, Scalar>;
using vec4f = vec4<float>;
using vec4d = vec4<double>;
using vec4l = vec4<long double>;

// Linear Algebra
template <std::size_t N, typename Scalar>
inline vec<N, Scalar> dot(const vec<N, Scalar>& lhs, const vec<N, Scalar>& rhs) {
    return lhs * rhs;
}

template <typename Scalar>
inline vec3<Scalar> cross(const vec3<Scalar>& lhs, const vec3<Scalar>& rhs) {
    return vec3<Scalar>({
        lhs[1] * rhs[2] - lhs[2] * rhs[1],
        lhs[2] * rhs[0] - lhs[0] * rhs[2],
        lhs[0] * rhs[1] - lhs[1] * rhs[0]
    });
}

template <typename Scalar>
inline Scalar dot_perp(const vec2<Scalar>& lhs, const vec2<Scalar>& rhs) {
    return lhs[0] * rhs[1] - lhs[1] * rhs[0];
}

template <std::size_t N, typename Scalar>
inline Scalar length_sqr(const vec<N, Scalar>& v) {
    return dot(v, v);
}

template <std::size_t N, typename Scalar>
inline Scalar length(const vec<N, Scalar>& v) {
    return std::sqrt(length_sqr(v));
}


// unary arithmetic operators
template <std::size_t N, typename Scalar>
inline vec<N, Scalar> operator+(const vec<N, Scalar>& rhs) {
    return rhs;
}

template <std::size_t N, typename Scalar>
inline vec<N, Scalar> operator-(const vec<N, Scalar>& rhs) {
    auto result = rhs;
    for (int i = 0; i < N; ++i)
        result[i] = -result[i];
    return result;
}

// elementwise binary arithmetic operators
template <std::size_t N, typename Scalar>
inline vec<N, Scalar> operator+(const vec<N, Scalar>& lhs, const vec<N, Scalar>& rhs) {
    return lhs += rhs;
}

template <std::size_t N, typename Scalar>
inline vec<N, Scalar> operator+=(const vec<N, Scalar>& lhs, const vec<N, Scalar>& rhs) {
    auto result = lhs;
    for (int i = 0; i < N; ++i)
        result[i] += rhs[i];
    return result;
}

template <std::size_t N, typename Scalar>
inline vec<N, Scalar> operator-(const vec<N, Scalar>& lhs, const vec<N, Scalar>& rhs) {
    return lhs -= rhs;
}

template <std::size_t N, typename Scalar>
inline vec<N, Scalar> operator-=(const vec<N, Scalar>& lhs, const vec<N, Scalar>& rhs) {
    auto result = lhs;
    for (int i = 0; i < N; ++i)
        result[i] -= rhs[i];
    return result;
}

template <std::size_t N, typename Scalar>
inline vec<N, Scalar> operator*(const vec<N, Scalar>& lhs, const vec<N, Scalar>& rhs) {
    return lhs *= rhs;
}

template <std::size_t N, typename Scalar>
inline vec<N, Scalar> operator*=(const vec<N, Scalar>& lhs, const vec<N, Scalar>& rhs) {
    auto result = lhs;
    for (int i = 0; i < N; ++i)
        result[i] *= rhs[i];
    return result;
}

template <std::size_t N, typename Scalar>
inline vec<N, Scalar> operator/(const vec<N, Scalar>& lhs, const vec<N, Scalar>& rhs) {
    return lhs /= rhs;
}

template <std::size_t N, typename Scalar>
inline vec<N, Scalar> operator/=(const vec<N, Scalar>& lhs, const vec<N, Scalar>& rhs) {
    auto result = lhs;
    for (int i = 0; i < N; ++i)
        result[i] /= rhs[i];
    return result;
}

// binary scalar arithmetic operators
template <std::size_t N, typename Scalar>
inline vec<N, Scalar> operator*(const vec<N, Scalar>& v, Scalar scalar) {
    return v *= scalar;
}

template <std::size_t N, typename Scalar>
inline vec<N, Scalar> operator*=(const vec<N, Scalar>& v, Scalar scalar) {
    auto result = v;
    for (int i = 0; i < N; ++i)
        result[i] *= scalar;
    return result;
}

template <std::size_t N, typename Scalar>
inline vec<N, Scalar> operator/(const vec<N, Scalar>& v, Scalar scalar) {
    return v /= scalar;
}

template <std::size_t N, typename Scalar>
inline vec<N, Scalar> operator/=(const vec<N, Scalar>& v, Scalar scalar) {
    auto result = v;
    for (int i = 0; i < N; ++i)
        result[i] /= scalar;
    return result;
}

}   // namespace mtlib

#endif // _MTLIB_ALGEBRA_VECTOR_H_