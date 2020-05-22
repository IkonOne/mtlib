#include "eigen_helpers.h"

#include <Eigen/Dense>

using namespace Eigen;

namespace mtlib {

double dot_perp(const Vector2d &v1, const Vector2d &v2) {
    return v1.coeff(0) * v2.coeff(1) - v1.coeff(1) * v2.coeff(0);
}

}   // mtlib