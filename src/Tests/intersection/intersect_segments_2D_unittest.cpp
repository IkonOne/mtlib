#include <MTLib/mtlib.h>

#include "gtest/gtest.h"

#include <cstddef>
#include <utility>
#include <vector>

using namespace mtlib;
using namespace std;

class IntersectSegments2DTest : public ::testing::Test {
protected:
    vector<segment2d> segments;
    vector<pair<vec2d, std::vector<segment2d>>> intersections;
};

TEST_F(IntersectSegments2DTest, None) {
    intersect_segments_2D(segments.begin(), segments.end(), back_inserter(intersections));

    ASSERT_TRUE(intersections.empty());
}

TEST_F(IntersectSegments2DTest, One) {
    segments.emplace_back(vec2d(0, 0), vec2d(1, 1));

    intersect_segments_2D(segments.begin(), segments.end(), back_inserter(intersections));

    ASSERT_TRUE(intersections.empty());
}

TEST_F(IntersectSegments2DTest, CrossX) {
    segments.emplace_back(vec2d(0, 0), vec2d(1, 1));
    segments.emplace_back(vec2d(1, 0), vec2d(0, 1));

    intersect_segments_2D(segments.begin(), segments.end(), back_inserter(intersections));

    ASSERT_EQ(1, intersections.size());
    ASSERT_EQ(2, intersections[0].second.size());
    EXPECT_EQ(vec2d(0.5, 0.5), intersections[0].first);
    EXPECT_NE(intersections[0].second[0], intersections[0].second[1]);
}

TEST_F(IntersectSegments2DTest, CrossXReverse) {
    segments.emplace_back(vec2d(0, 0), vec2d(-1, -1));
    segments.emplace_back(vec2d(-1, 0), vec2d(0, -1));

    intersect_segments_2D(segments.begin(), segments.end(), back_inserter(intersections));

    ASSERT_EQ(1, intersections.size());
    ASSERT_EQ(2, intersections[0].second.size());
    EXPECT_EQ(vec2d(-0.5, -0.5), intersections[0].first);
    EXPECT_NE(intersections[0].second[0], intersections[0].second[1]);
}

TEST_F(IntersectSegments2DTest, CrossT) {
    segments.emplace_back(vec2d(-1, 0), vec2d(1, 0));
    segments.emplace_back(vec2d(0, -1), vec2d(0, 1));

    intersect_segments_2D(segments.begin(), segments.end(), back_inserter(intersections));

    ASSERT_EQ(1, intersections.size());
    ASSERT_EQ(2, intersections[0].second.size());
    EXPECT_EQ(vec2d(0, 0), intersections[0].first);
    EXPECT_NE(intersections[0].second[0], intersections[0].second[1]);
}

TEST_F(IntersectSegments2DTest, Colinear) {
    segments.emplace_back(vec2d(0, 0), vec2d(0, 1));
    segments.emplace_back(vec2d(0, 1), vec2d(0, 2));

    intersect_segments_2D(segments.begin(), segments.end(), back_inserter(intersections));

    ASSERT_EQ(1, intersections.size());
    ASSERT_EQ(2, intersections[0].second.size());
    EXPECT_EQ(vec2d(0, 1), intersections[0].first);
    EXPECT_NE(intersections[0].second[0], intersections[0].second[1]);
}

TEST_F(IntersectSegments2DTest, Triangle) {
    segments.emplace_back(vec2d(-2, -2), vec2d(2, 2));
    segments.emplace_back(vec2d(-2, 2), vec2d(2, 2));
    segments.emplace_back(vec2d(-2, -2), vec2d(-2, 2));

    intersect_segments_2D(segments.begin(), segments.end(), back_inserter(intersections));

    EXPECT_EQ(3, intersections.size());

    for (auto& intersection : intersections)
        EXPECT_EQ(2, intersection.second.size());

    for (auto& intersection : intersections)
        EXPECT_NE(intersection.second[0], intersection.second[1]);
}

TEST_F(IntersectSegments2DTest, TriangleOverlaps) {
    segments.emplace_back(vec2d(-4, -4), vec2d(4, 4));
    segments.emplace_back(vec2d(-4, 2), vec2d(4, 2));
    segments.emplace_back(vec2d(-2, -4), vec2d(-2, 4));

    intersect_segments_2D(segments.begin(), segments.end(), back_inserter(intersections));

    EXPECT_EQ(3, intersections.size());

    for (auto& intersection : intersections)
        EXPECT_EQ(2, intersection.second.size());

    for (auto& intersection : intersections)
        EXPECT_NE(intersection.second[0], intersection.second[1]);
}