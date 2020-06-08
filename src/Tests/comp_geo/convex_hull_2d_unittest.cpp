#include <MTLib/mtlib.h>

#include <gtest/gtest.h>

#include <iostream>
#include <vector>

using namespace std;
using namespace mtlib;

namespace {

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