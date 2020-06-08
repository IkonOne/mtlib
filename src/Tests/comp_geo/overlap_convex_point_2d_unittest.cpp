#include <MTLib/mtlib.h>

#include <gtest/gtest.h>

using namespace mtlib;
using namespace std;

namespace {

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

}