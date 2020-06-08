#include <MTLib/mtlib.h>

#include <gtest/gtest.h>

#include <vector>

using namespace mtlib;
using namespace std;


namespace {

TEST(IsConvex2dPHullDeathTest, ZeroPoints) {
    vector<vec2d> empty_hull;

    ASSERT_DEATH({
        is_convex_2d(empty_hull.begin(), empty_hull.end());
    }, "first != last");
}

TEST(IsConvex2dPHullTest, Point) {
    vector<vec2d> point;
    point.emplace_back(0, 1);

    EXPECT_TRUE(is_convex_2d(point.begin(), point.end()));
}

TEST(IsConvex2dPHullTest, Line) {
    vector<vec2d> line;
    line.emplace_back(0, 1);
    line.emplace_back(1, 0);

    EXPECT_TRUE(is_convex_2d(line.begin(), line.end()));
}

TEST(IsConvex2dPHullTest, TriangleCWFails) {
    vector<vec2d> tri_cw;
    tri_cw.emplace_back(0, 0);
    tri_cw.emplace_back(0, 1);
    tri_cw.emplace_back(1, 1);

    EXPECT_FALSE(is_convex_2d(tri_cw.begin(), tri_cw.end()));
}

TEST(IsConvex2dPHullTest, TriangleCCW) {
    vector<vec2d> tri_ccw;
    tri_ccw.emplace_back(0, 0);
    tri_ccw.emplace_back(1, 0);
    tri_ccw.emplace_back(1, 1);

    EXPECT_TRUE(is_convex_2d(tri_ccw.begin(), tri_ccw.end()));
}

TEST(IsConvex2dPHullTest, QuadCWFails) {
    vector<vec2d> quad_cw;
    quad_cw.emplace_back(0, 0);
    quad_cw.emplace_back(0, 1);
    quad_cw.emplace_back(1, 1);
    quad_cw.emplace_back(1, 0);

    EXPECT_FALSE(is_convex_2d(quad_cw.begin(), quad_cw.end()));
}

TEST(IsConvex2dPHullTest, QuadCCW) {
    vector<vec2d> quad_ccw;
    quad_ccw.emplace_back(0, 0);
    quad_ccw.emplace_back(1, 0);
    quad_ccw.emplace_back(1, 1);
    quad_ccw.emplace_back(0, 1);

    EXPECT_TRUE(is_convex_2d(quad_ccw.begin(), quad_ccw.end()));
}

TEST(IsConvex2dPHullTest, BowTieFails) {
    /**
     * (0,1)*-----*(1,1)
     *       \   /
     *        \ /
     *         x
     *        / \
     *       /   \
     * (0,0)*-----*(1,0)
     */

    vector<vec2d> bow_tie;
    bow_tie.emplace_back(0, 0);
    bow_tie.emplace_back(1, 0);
    bow_tie.emplace_back(0, 1);
    bow_tie.emplace_back(1, 1);

    EXPECT_FALSE(is_convex_2d(bow_tie.begin(), bow_tie.end()));
}

}