#ifndef _MTLIB_GEOMETRY_SEGMENT_H_
#define _MTLIB_GEOMETRY_SEGMENT_H_

#include "MTLib/algebra/vec.h"

#include <array>
#include <algorithm>
#include <cstddef>  // size_t
#include <initializer_list>

namespace mtlib {

template <std::size_t N, typename Scalar>
class segment {
public:
    static constexpr std::size_t rank = N;
    using vec_type = vec<N, Scalar>;
    using value_type = vec_type;
    using scalar_type = typename vec_type::scalar_type;
    using reference = vec_type&;
    using const_reference = const reference;
    using iterator = typename std::array<vec_type, 2>::iterator;
    using const_iterator = typename std::array<vec_type, 2>::const_iterator;

    constexpr iterator begin() noexcept { return endpoints.begin(); }
    constexpr iterator end() noexcept { return endpoints.end(); }

public:
    constexpr segment() = default;
    constexpr segment(std::array<vec<N, Scalar>, 2> ends) : endpoints(ends) {}
    constexpr segment(const vec<N, Scalar>& v1, const vec<N, Scalar>& v2)
        : endpoints({v1, v2})
    {}

    // accessors
    constexpr vec<N, Scalar>& at(std::size_t idx) { return endpoints[idx]; }
    constexpr const vec<N, Scalar>& at(std::size_t idx) const { return endpoints[idx]; }

    constexpr vec<N, Scalar> &operator[](std::size_t idx) { return endpoints[idx]; }
    constexpr const vec<N, Scalar>& operator[](std::size_t idx) const { return endpoints[idx]; }

    // lexicographic comparisons
    constexpr bool operator==(const vec<N, Scalar>& rhs) const { return endpoints == rhs.endpoints; }
    constexpr bool operator!=(const vec<N, Scalar>& rhs) const { return endpoints != rhs.endpoints; }
    constexpr bool operator< (const vec<N, Scalar>& rhs) const { return endpoints <  rhs.endpoints; }
    constexpr bool operator<=(const vec<N, Scalar>& rhs) const { return endpoints <= rhs.endpoints; }
    constexpr bool operator> (const vec<N, Scalar>& rhs) const { return endpoints >  rhs.endpoints; }
    constexpr bool operator>=(const vec<N, Scalar>& rhs) const { return endpoints >= rhs.endpoints; }

private:
    std::array<vec<N, Scalar>, 2> endpoints;
};

// common aliases
template <typename Scalar>
using segment2 = segment<2, Scalar>;
using segment2f = segment2<float>;
using segment2d = segment2<double>;
using segment2l = segment2<long double>;

template <typename Scalar>
using segment3 = segment<3, Scalar>;
using segment3f = segment3<float>;
using segment3d = segment3<double>;
using segment3l = segment3<long double>;

template <typename Scalar>
using segment4 = segment<4, Scalar>;
using segment4f = segment4<float>;
using segment4d = segment4<double>;
using segment4l = segment4<long double>;

template <std::size_t N, typename Scalar>
constexpr const vec<N, Scalar>& min_endpoint(const segment<N, Scalar>& seg) {
    return std::min(seg[0], seg[1]);
}

template <std::size_t N, typename Scalar>
constexpr const vec<N, Scalar>& max_endpoint(const segment<N, Scalar>& seg) {
    return std::max(seg[0], seg[1]);
}

template <std::size_t N, typename Scalar>
constexpr Scalar min_on_dim(const segment<N, Scalar>& seg, std::size_t dim) {
    return std::min(seg[0][dim], seg[1][dim]);
}

template <std::size_t N, typename Scalar>
constexpr Scalar max_on_dim(const segment<N, Scalar>& seg, std::size_t dim) {
    return std::max(seg[0][dim], seg[1][dim]);
}

template <std::size_t N, typename Scalar>
constexpr vec<N, Scalar> evaluate_at_t(const segment<N, Scalar>& seg, Scalar t) {
    vec<N, Scalar> v;
    for (int i = 0; i < N; ++i)
        v[i] = seg[0][i] + (seg[1][i] - seg[0][i]) * t;
    return v;
}

}   // namespace mtlib

#endif // _MTLIB_GEOMETRY_SEGMENT_H_