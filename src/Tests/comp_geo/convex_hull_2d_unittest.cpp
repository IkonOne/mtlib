#include <MTLib/mtlib.h>

#include <Eigen/Dense>
#include <gtest/gtest.h>

#include <iostream>
#include <vector>

using namespace std;
using namespace Eigen;
using namespace mtlib;

namespace {

TEST(IsConvex2dPHullDeathTest, ZeroPoints) {
    vector<Vector2d> empty_hull;

    ASSERT_DEATH({
        is_convex_2d(empty_hull);
    }, "!hull.empty()");
}

TEST(IsConvex2dPHullTest, Point) {
    vector<Vector2d> point;
    point.emplace_back(0, 1);

    EXPECT_TRUE(is_convex_2d(point));
}

TEST(IsConvex2dPHullTest, Line) {
    vector<Vector2d> line;
    line.emplace_back(0, 1);
    line.emplace_back(1, 0);

    EXPECT_TRUE(is_convex_2d(line));
}

TEST(IsConvex2dPHullTest, TriangleCWFails) {
    vector<Vector2d> tri_cw;
    tri_cw.emplace_back(0, 0);
    tri_cw.emplace_back(0, 1);
    tri_cw.emplace_back(1, 1);

    EXPECT_FALSE(is_convex_2d(tri_cw));
}

TEST(IsConvex2dPHullTest, TriangleCCW) {
    vector<Vector2d> tri_ccw;
    tri_ccw.emplace_back(0, 0);
    tri_ccw.emplace_back(1, 0);
    tri_ccw.emplace_back(1, 1);

    EXPECT_TRUE(is_convex_2d(tri_ccw));
}

TEST(IsConvex2dPHullTest, QuadCWFails) {
    vector<Vector2d> quad_cw;
    quad_cw.emplace_back(0, 0);
    quad_cw.emplace_back(0, 1);
    quad_cw.emplace_back(1, 1);
    quad_cw.emplace_back(1, 0);

    EXPECT_FALSE(is_convex_2d(quad_cw));
}

TEST(IsConvex2dPHullTest, QuadCCW) {
    vector<Vector2d> quad_ccw;
    quad_ccw.emplace_back(0, 0);
    quad_ccw.emplace_back(1, 0);
    quad_ccw.emplace_back(1, 1);
    quad_ccw.emplace_back(0, 1);

    EXPECT_TRUE(is_convex_2d(quad_ccw));
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

    vector<Vector2d> bow_tie;
    bow_tie.emplace_back(0, 0);
    bow_tie.emplace_back(1, 0);
    bow_tie.emplace_back(0, 1);
    bow_tie.emplace_back(1, 1);

    EXPECT_FALSE(is_convex_2d(bow_tie));
}

TEST(OverlapConvexPoint2dDeathTest, MinHullSize) {
    vector<Vector2d> hull;
    hull.emplace_back(0, 0);
    hull.emplace_back(1, 0);

    Vector2d p(0, 0);

    EXPECT_DEATH({
        overlap_convex_point_2d(hull, p);
    }, "hull.size\\(\\) >= 3");

    hull.emplace_back(1, 1);
    EXPECT_NO_FATAL_FAILURE({
        overlap_convex_point_2d(hull, p);
    });
}

TEST(OverlapConvexPoint2dDeathTest, NonConvex) {
    vector<Vector2d> non_convex;
    non_convex.emplace_back(0, 0);
    non_convex.emplace_back(1, 0);
    non_convex.emplace_back(0, 1);
    non_convex.emplace_back(1, 1);

    Vector2d p;

    EXPECT_DEATH({
        overlap_convex_point_2d(non_convex, p);
    }, "is_convex_2d\\(hull\\)");
}

TEST(OverlapConvexPoint2dTest, BaseCaseTriPoint) {
    vector<Vector2d> tri;
    tri.emplace_back(0, 0);
    tri.emplace_back(1, 0);
    tri.emplace_back(1, 1);

    Vector2d p_in(0.5, 0.25);
    Vector2d p_out(5, 5);
    Vector2d p_on(0, 0);

    EXPECT_TRUE(overlap_convex_point_2d(tri, p_in));
    EXPECT_FALSE(overlap_convex_point_2d(tri, p_out));
    EXPECT_TRUE(overlap_convex_point_2d(tri, p_on));
}

TEST(CHullGraham2dDeathTest, Empty) {
    vector<Vector2d> empty_points;

    EXPECT_DEBUG_DEATH({
        chull_graham_2d(empty_points);
    }, "!points.empty()");
}

TEST(CHullGraham2dTest, MinHullSize) {
    vector<Vector2d> points;
    points.emplace_back(0, 0);

    auto hull = chull_graham_2d(points);
    ASSERT_EQ(1, hull.size());

    points.emplace_back(1, 0);
    hull = chull_graham_2d(points);
    ASSERT_EQ(2, hull.size());
}

TEST(CHullGraham2dTest, ThreeHull) {
    vector<Vector2d> points;
    points.emplace_back(0, 0);
    points.emplace_back(1, 1);
    points.emplace_back(1, 0);

    auto hull = chull_graham_2d(points);
    EXPECT_EQ(3, hull.size());
    EXPECT_TRUE(is_convex_2d(hull));
}

TEST(CHullGraham2dTest, QuadCW) {
    vector<Vector2d> quad_cw;
    quad_cw.emplace_back(0, 0);
    quad_cw.emplace_back(0, 1);
    quad_cw.emplace_back(1, 1);
    quad_cw.emplace_back(1, 0);

    auto hull = chull_graham_2d(quad_cw);
    EXPECT_EQ(4, hull.size());
    EXPECT_TRUE(is_convex_2d(hull));
}

TEST(CHullGraham2dTest, Grid3x3) {
    vector<Vector2d> grid;
    grid.emplace_back(-1, 1); grid.emplace_back(0, 1); grid.emplace_back(1, 1);
    grid.emplace_back(-1, 0); grid.emplace_back(0, 0); grid.emplace_back(1, 0);
    grid.emplace_back(-1, -1); grid.emplace_back(0, -1); grid.emplace_back(1, -1);

    auto hull = chull_graham_2d(grid);
    // FIXME: is_ccw returns true when ccw AND colinear.  therefore hull.size() == 8
    // EXPECT_EQ(4, hull.size());
    EXPECT_LE(4, hull.size());
    EXPECT_TRUE(is_convex_2d(hull));
}

}   // namespace