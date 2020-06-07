#ifndef _MTLIB_COMP_GEO_CONVEX_HULL_2D_H_
#define _MTLIB_COMP_GEO_CONVEX_HULL_2D_H_

#include "MTLib/algebra/vec.h"
#include "MTLib/algebra/linalg.h"

#include <cassert>
#include <iostream>
#include <iterator>
#include <type_traits>
#include <vector>

namespace mtlib {

bool is_convex_2d(const std::vector<vec2d>& hull);
bool overlap_convex_point_2d(const std::vector<vec2d>& hull, const vec2d& p);

template <typename RandomIt>
std::vector<vec2d> chull_graham_2d(RandomIt first, RandomIt last) {
    assert(distance(first, last) > 0);

    // auto is_ccw = [](const vec2d& v1, const vec2d& v2, const vec2d& v3) {
    //     return dot_perp(v2 - v1, v3 - v1) > 0;
    // };

    int n = distance(first, last);
    std::vector<vec2d> result;

    if (n <= 3) {
        for (; first != last; ++first)
            result.push_back(*first);
        
        if (result.size() == 3 && !is_ccw(result[0], result[1], result[2]))
            iter_swap(result.begin(), result.begin() + 1);
    }
    else {
        std::vector<vec2d> cp(n);
        copy(first, last, cp.begin());
        sort(cp.begin(), cp.end(), [](auto lhs, auto rhs) {
            // lexicographic
            if (lhs[0] < rhs[0]) return true;
            if (lhs[0] > rhs[0]) return false;
            return lhs[1] < rhs[1];
        });

        // bottom hull
        result.push_back(cp[0]);
        result.push_back(cp[1]);
        for (int i = 2; i < cp.size(); ++i) {
            result.push_back(cp[i]);
            while (result.size() > 2 && !is_ccw(result[result.size() - 3], result[result.size() - 2], result[result.size() - 1]))
                result.erase(result.end() - 2);
        }

        // top hull
        std::vector<vec2d> rtop;
        rtop.push_back(cp[cp.size() - 1]);
        rtop.push_back(cp[cp.size() - 2]);
        for (int i = cp.size() - 3; i >= 0; --i) {
            rtop.push_back(cp[i]);
            while (rtop.size() > 2 && !is_ccw(rtop[rtop.size() - 3], rtop[rtop.size() - 2], rtop[rtop.size() - 1]))
                rtop.erase(rtop.end() - 2);
        }

        copy_n(rtop.begin() + 1, rtop.size() - 2, std::back_inserter(result));
    }

    return result;
}

}

#endif // _MTLIB_CONVEX_HULL_2D_H_