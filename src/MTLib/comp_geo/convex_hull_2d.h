#ifndef _MTLIB_COMP_GEO_CONVEX_HULL_2D_H_
#define _MTLIB_COMP_GEO_CONVEX_HULL_2D_H_

#include <Eigen/Dense>

#include <vector>
#include <iostream>
#include <iterator>
#include <type_traits>

namespace mtlib {

bool is_convex_2d(const std::vector<Eigen::Vector2d> &hull);
bool overlap_convex_point_2d(const std::vector<Eigen::Vector2d> &hull, const Eigen::Vector2d &p);
std::vector<Eigen::Vector2d> chull_graham_2d(const std::vector<Eigen::Vector2d> &points);

}

#endif // _MTLIB_CONVEX_HULL_2D_H_