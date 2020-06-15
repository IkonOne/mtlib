#ifndef _MTLIB_CONVEX_HULL_2D_H_
#define _MTLIB_CONVEX_HULL_2D_H_

#include "MTLib/algebra/vec.h"
#include "MTLib/algebra/linalg.h"
#include "MTLib/comp_geo/overlap_convex_point_2d.h"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <type_traits>
#include <vector>

namespace mtlib {

template <typename RandomIt, typename OutputIt, typename Scalar = typename RandomIt::value_type::scalar_type>
void chull_graham_2d(const RandomIt& first, const RandomIt& last, const OutputIt& d_first) {
    assert(distance(first, last) > 0);

    int n = distance(first, last);
    std::vector<vec2<Scalar>> result;

    if (n <= 3) {
        for (auto it = first; it != last; ++it)
            result.push_back(*it);
        
        if (result.size() == 3 && !is_ccw(result[0], result[1], result[2]))
            std::swap(result[0], result[1]);

        std::copy(result.begin(), result.end(), d_first);
    }
    else {
        std::vector<vec2<Scalar>> cp(n);
        std::copy(first, last, cp.begin());
        std::sort(cp.begin(), cp.end());

        // bottom hull
        result.push_back(cp[0]);
        result.push_back(cp[1]);
        for (std::size_t i = 2; i < cp.size(); ++i) {
            result.push_back(cp[i]);
            while (result.size() > 2 && !is_ccw(result[result.size() - 3], result[result.size() - 2], result[result.size() - 1]))
                result.erase(result.end() - 2);
        }
        std::move(result.begin(), result.end(), d_first);

        // top hull
        result.clear();
        result.push_back(cp[cp.size() - 1]);
        result.push_back(cp[cp.size() - 2]);
        for (int i = cp.size() - 3; i >= 0; --i) {
            result.push_back(cp[i]);
            while (result.size() > 2 && !is_ccw(result[result.size() - 3], result[result.size() - 2], result[result.size() - 1]))
                result.erase(result.end() - 2);
        }
        std::move(result.begin() + 1, result.end() - 1, d_first);
    }
}

}

#endif // _MTLIB_CONVEX_HULL_2D_H_