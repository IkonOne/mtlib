#ifndef _MTLIB_CONVEX_HULL_2D_H_
#define _MTLIB_CONVEX_HULL_2D_H_

#include <Eigen/Dense>

#include <vector>
#include <iostream>
#include <iterator>
#include <type_traits>

namespace mtlib {

bool is_convex_2d(const std::vector<Eigen::Vector2d> &points);
bool overlap_convex_point_2d(const std::vector<Eigen::Vector2d> &hull, const Eigen::Vector2d &p);

}

#endif // _MTLIB_CONVEX_HULL_2D_H_