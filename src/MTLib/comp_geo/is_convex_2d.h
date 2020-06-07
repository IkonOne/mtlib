#ifndef _MTLIB_IS_CONVEX_2D_H_
#define _MTLIB_IS_CONVEX_2D_H_

#include "MTLib/algebra/linalg.h"

#include <iterator>
#include <cassert>

namespace mtlib {

template <typename RandomIt>
inline bool is_convex_2d(const RandomIt& first, const RandomIt& last) {
    assert(first != last);

    int n = std::distance(first, last);
    if (n < 3)
        return true;

    for (int i = 0; i < n; i += 2) {
        if (!is_ccw(first[i], first[(i + 1) % n], first[(i + 2) % n]))
            return false;
    }

    return true;
}

}

#endif // _MTLIB_IS_CONVEX_2D_H_