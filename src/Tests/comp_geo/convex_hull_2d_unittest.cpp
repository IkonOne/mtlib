#include <MTLib/mtlib.h>

#include <gtest/gtest.h>

#include <iostream>
#include <vector>

using namespace std;
using namespace mtlib;

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

TEST(OverlapConvexPoint2dDeathTest, MinHullSize) {
    vector<vec2d> hull;
    hull.emplace_back(0, 0);
    hull.emplace_back(1, 0);

    vec2d p(0, 0);

    EXPECT_DEATH({
        overlap_convex_point_2d(hull.begin(), hull.end(), p);
    }, "distance\\(.*first.*last.*\\) >= 3");

    hull.emplace_back(1, 1);
    EXPECT_NO_FATAL_FAILURE({
        overlap_convex_point_2d(hull.begin(), hull.end(), p);
    });
}

TEST(OverlapConvexPoint2dDeathTest, NonConvex) {
    vector<vec2d> non_convex;
    non_convex.emplace_back(0, 0);
    non_convex.emplace_back(1, 0);
    non_convex.emplace_back(0, 1);
    non_convex.emplace_back(1, 1);

    vec2d p;

    EXPECT_DEATH({
        overlap_convex_point_2d(non_convex.begin(), non_convex.end(), p);
    }, "is_convex_2d\\(.*first.*last.*\\)");
}

TEST(OverlapConvexPoint2dTest, BaseCaseTriPoint) {
    vector<vec2d> tri;
    tri.emplace_back(0, 0);
    tri.emplace_back(1, 0);
    tri.emplace_back(1, 1);

    vec2d p_in(0.5, 0.25);
    vec2d p_out(5, 5);
    vec2d p_on(0, 0);

    EXPECT_TRUE(overlap_convex_point_2d(tri.begin(), tri.end(), p_in));
    EXPECT_FALSE(overlap_convex_point_2d(tri.begin(), tri.end(), p_out));
    EXPECT_TRUE(overlap_convex_point_2d(tri.begin(), tri.end(), p_on));
}

class CHullGraham2dTest : public ::testing::Test {
protected:
    vector<vec2d> chull;
};

class CHullGraham2dDeathTest : public CHullGraham2dTest {};

TEST_F(CHullGraham2dDeathTest, Empty) {
    vector<vec2d> empty_points;

    EXPECT_DEBUG_DEATH({
        chull_graham_2d(empty_points.begin(), empty_points.end(), back_inserter(chull));
    }, "distance\\(first, last\\) > 0");
}

TEST_F(CHullGraham2dTest, MinHullSize) {
    vector<vec2d> points;
    points.emplace_back(0, 0);

    chull_graham_2d(points.begin(), points.end(), back_inserter(chull));
    ASSERT_EQ(1, chull.size());

    chull.clear();
    points.emplace_back(1, 0);
    chull_graham_2d(points.begin(), points.end(), back_inserter(chull));
    ASSERT_EQ(2, chull.size());
}

TEST_F(CHullGraham2dTest, ThreeHull) {
    vector<vec2d> points;
    points.emplace_back(0, 0);
    points.emplace_back(1, 1);
    points.emplace_back(1, 0);

    chull_graham_2d(points.begin(), points.end(), back_inserter(chull));
    EXPECT_EQ(3, chull.size());
    EXPECT_TRUE(is_convex_2d(chull.begin(), chull.end()));
}

TEST_F(CHullGraham2dTest, QuadCW) {
    vector<vec2d> quad_cw;
    quad_cw.emplace_back(0, 0);
    quad_cw.emplace_back(0, 1);
    quad_cw.emplace_back(1, 1);
    quad_cw.emplace_back(1, 0);

    chull_graham_2d(quad_cw.begin(), quad_cw.end(), back_inserter(chull));
    EXPECT_EQ(4, chull.size());
    EXPECT_TRUE(is_convex_2d(chull.begin(), chull.end()));
}

TEST_F(CHullGraham2dTest, EdgeCase1) {
    vector<vec2d> points;
    points.emplace_back(-1, 0);
    points.emplace_back(0, -1);
    points.emplace_back(0, 0);
    points.emplace_back(1, 1);

    chull_graham_2d(points.begin(), points.end(), back_inserter(chull));
    EXPECT_EQ(3, chull.size());
    EXPECT_TRUE(is_convex_2d(chull.begin(), chull.end()));
}

TEST_F(CHullGraham2dTest, Grid3x3) {
    vector<vec2d> grid;
    grid.emplace_back(-1, 1); grid.emplace_back(0, 1); grid.emplace_back(1, 1);
    grid.emplace_back(-1, 0); grid.emplace_back(0, 0); grid.emplace_back(1, 0);
    grid.emplace_back(-1, -1); grid.emplace_back(0, -1); grid.emplace_back(1, -1);

    chull_graham_2d(grid.begin(), grid.end(), back_inserter(chull));
    EXPECT_EQ(4, chull.size());
    EXPECT_TRUE(is_convex_2d(chull.begin(), chull.end()));
}

}   // namespace