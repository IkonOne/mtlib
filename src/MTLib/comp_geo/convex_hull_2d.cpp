#include "convex_hull_2d.h" 
#include "../algebra/eigen_helpers.h"

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
// consider classifying ccw, cw and colinear
// returns true if CCW, false if CW
inline bool is_ccw(const Vector2d &p1, const Vector2d &p2, const Vector2d &p3) {
    return dot_perp(p2 - p1, p3 - p1) >= 0;
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
    if (dot_perp(p2 - p1, p - p1) > 0)
        copy(hull.begin(), hull.begin() + mid, sub_hull.begin());
    else
        copy(hull.begin() + (hull.size() > 4 ? mid : 1), hull.end(), sub_hull.begin());

    return overlap_convex_point_2d(sub_hull, p);
}

}   // mtlib