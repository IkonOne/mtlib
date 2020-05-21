#ifndef _EIGEN_HELPERS_H_
#define _EIGEN_HELPERS_H_

#include <Eigen/Dense>

namespace mtlib {

double dot_perp(const Eigen::Vector2d &v1, const Eigen::Vector2d &v2);

}   // mtlib

#endif // _EIGEN_HELPERS_H_