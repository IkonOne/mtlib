#ifndef _MTLIB_COMP_GEO_CONVEX_HULL_2D_H_
#define _MTLIB_COMP_GEO_CONVEX_HULL_2D_H_

#include "../algebra/eigen_helpers.h"

#include <Eigen/Dense>

#include <cassert>
#include <iostream>
#include <iterator>
#include <type_traits>
#include <vector>

namespace mtlib {

bool is_convex_2d(const std::vector<Eigen::Vector2d> &hull);
bool overlap_convex_point_2d(const std::vector<Eigen::Vector2d> &hull, const Eigen::Vector2d &p);

template <typename RandomIt>
std::vector<Eigen::Vector2d> chull_graham_2d(RandomIt first, RandomIt last) {
    assert(distance(first, last) > 0);

    auto is_ccw = [](const Eigen::Vector2d &v1, const Eigen::Vector2d &v2, const Eigen::Vector2d &v3) {
        return dot_perp(v2 - v1, v3 - v1) > 0;
    };

    int n = distance(first, last);
    std::vector<Eigen::Vector2d> result;

    if (n <= 3) {
        for (; first != last; ++first)
            result.push_back(*first);
        
        if (result.size() == 3 && !is_ccw(result[0], result[1], result[2]))
            iter_swap(result.begin(), result.begin() + 1);
    }
    else {
        std::vector<Eigen::Vector2d> cp(n);
        copy(first, last, cp.begin());
        sort(cp.begin(), cp.end(), [](auto lhs, auto rhs) {
            // lexicographic
            if (lhs.coeff(0) < rhs.coeff(0)) return true;
            if (lhs.coeff(0) > rhs.coeff(0)) return false;
            return lhs.coeff(1) < rhs.coeff(1);
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
        std::vector<Eigen::Vector2d> rtop;
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