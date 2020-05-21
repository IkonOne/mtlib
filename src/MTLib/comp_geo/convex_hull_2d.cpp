#include "convex_hull_2d.h" 

#include <Eigen/Dense>
#include <Eigen/Geometry>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <vector>

#include <iostream>

using namespace std;
using namespace Eigen;

/**
 * Counter-Clockwise Winding
 **/

namespace mtlib {

// TODO: Refactor to general API
inline double cross_2d(const Vector2d &lhs, const Vector2d &rhs) {
    return lhs.coeff(0) * rhs.coeff(1) - lhs.coeff(1) * rhs.coeff(0);
}

// TODO: Refactor to general API
// consider classifying ccw, cw and colinear
// returns true if CCW, false if CW
inline bool is_ccw(const Vector2d &p1, const Vector2d &p2, const Vector2d &p3) {
    return cross_2d(p2 - p1, p3 - p1) >= 0;
}

bool is_convex_2d(const vector<Vector2d> &hull) {
    assert(!hull.empty());

    if (hull.size() < 3)
        return true;
    
    for (int i = 0; i < hull.size(); ++i) {
        if (!is_ccw(hull[i], hull[(i + 1) % hull.size()], hull[(i + 2) % hull.size()]))
            return false;
    }

    return true;
}

bool overlap_convex_point_2d(const vector<Vector2d> &hull, const Vector2d &p) {
    assert(hull.size() >= 3);
    assert(is_convex_2d(hull));

    if (hull.size() == 3) {
        return (
            is_ccw(hull[0], hull[1], p) &&
            is_ccw(hull[1], hull[2], p) &&
            is_ccw(hull[2], hull[0], p)
        );
    }

    int mid = hull.size() / 2;
    auto p1 = hull[0];
    auto p2 = hull[mid];

    vector<Vector2d> sub_hull;
    if (cross_2d(p2 - p1, p - p1) > 0)
        copy(hull.begin(), hull.begin() + mid, sub_hull.begin());
    else
        copy(hull.begin() + (hull.size() > 4 ? mid : 1), hull.end(), sub_hull.begin());

    return overlap_convex_point_2d(sub_hull, p);
}

vector<Vector2d> chull_graham_2d(const vector<Vector2d> &points) {
    assert(!points.empty());

    vector<Vector2d> result;

    if (points.size() <= 3) {
        for (auto p : points)
            result.push_back(p);
        
        if (result.size() == 3 && !is_ccw(result[0], result[1], result[2]))
            swap(result[1], result[2]);
        
        return result;
    }

    vector<Vector2d> cp;
    copy(points.begin(), points.end(), back_insert_iterator<vector<Vector2d>>(cp));

    // lexicographic Vector2d Comparator
    sort(cp.begin(), cp.end(), [](auto &lhs, auto &rhs) {
        if (lhs.coeff(0) < rhs.coeff(0)) return true;
        if (lhs.coeff(0) > rhs.coeff(0)) return false;
        return lhs.coeff(1) < rhs.coeff(1);
    });

    // lower half of hull from far left
    result.push_back(cp[0]);
    result.push_back(cp[1]);
    for (int i = 2; i < cp.size(); ++i) {
        result.push_back(cp[i]);
        while (result.size() >= 3 &&
                !is_ccw(result[result.size() - 3], result[result.size() - 2], result[result.size() - 1]))
            result.erase(result.begin() + result.size() - 2);
    }

    // upper half from far right
    for (int i = cp.size() - 2; i > 0; --i) {
        result.push_back(cp[i]);
        while (result.size() >= 3 &&
                !is_ccw(result[result.size() - 3], result[result.size() - 2], result[result.size() - 1]))
            result.erase(result.begin() + result.size() - 2);
    }

    return result;
}

}