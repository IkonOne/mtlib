#ifndef _MTLIB_ALGEBRA_EIGEN_HELPERS_H_
#define _MTLIB_ALGEBRA_EIGEN_HELPERS_H_

#include <Eigen/Dense>

namespace mtlib {

double dot_perp(const Eigen::Vector2d &v1, const Eigen::Vector2d &v2);

}   // mtlib

#endif // _MTLIB_ALGEBRA_EIGEN_HELPERS_H_