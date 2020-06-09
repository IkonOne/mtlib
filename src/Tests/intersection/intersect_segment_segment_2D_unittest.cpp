#include <MTLib/mtlib.h>

#include <gtest/gtest.h>

using namespace mtlib;
using namespace std;

// Overlap tests

TEST(OverlapSegmentSegment2DTest, OverlapX) {
    segment2d s1 = { {0, 1}, {1, 0} };
    segment2d s2 = { {0, 0}, {1, 1} };

    ASSERT_TRUE(overlap_segment_segment_2D(s1, s2));
}

TEST(OverlapSegmentSegment2DTest, OverlapCross) {
    segment2d s1 = { {-1, 0}, {1, 0} };
    segment2d s2 = { {0, -1}, {0, 1} };

    ASSERT_TRUE(overlap_segment_segment_2D(s1, s2));
}

TEST(OverlapSegmentSegment2DTest, OverlapCoincident) {
    segment2d s1 = { {0,  0}, {1, 0} };
    segment2d s2 = { {0, -1}, {0, 1} };

    ASSERT_TRUE(overlap_segment_segment_2D(s1, s2));
}

TEST(OverlapSegmentSegment2DTest, OverlapPointCoincident) {
    segment2d s1 = { {0, -1}, {1, 0} };
    segment2d s2 = { {0, -1}, {0, 1} };

    ASSERT_TRUE(overlap_segment_segment_2D(s1, s2));
}

TEST(OverlapSegmentSegment2DTest, OverlapColinear) {
    segment2d s1 = { {0,  0}, {0, 2} };
    segment2d s2 = { {0, -1}, {0, 1} };

    ASSERT_TRUE(overlap_segment_segment_2D(s1, s2));
}

TEST(OverlapSegmentSegment2DTest, NoOverlapColinear) {
    segment<2, double> s1 = { {0,  2}, {0, 3} };
    segment<2, double> s2 = { {0, -1}, {0, 1} };

    ASSERT_FALSE(overlap_segment_segment_2D(s1, s2));
}

// Intersection tests

TEST(IntersectSegmentSegment2DTest, ResultInvariant) {
    segment2d s1 = { {0, 1}, {1, 0} };
    segment2d s2 = { {0, 0}, {1, 1} };

    const auto [result, success] = intersect_segment_segment_2D(s1, s2);
    ASSERT_EQ(s1, result.segment1);
    ASSERT_EQ(s2, result.segment2);
}

TEST(IntersectSegmentSegment2DTest, OverlapingX) {
    segment2d s1 = { {0, 1}, {1, 0} };
    segment2d s2 = { {0, 0}, {1, 1} };

    const auto [result, success] = intersect_segment_segment_2D(s1, s2);
    ASSERT_TRUE(success);
    EXPECT_EQ(0.5, result.t1);
    EXPECT_EQ(0.5, result.t2);
    EXPECT_EQ(vec2d(0.5, 0.5), result.point);
}

TEST(IntersectSegmentSegment2DTest, OverlapingCross) {
    segment2d s1 = { {-1, 0.5}, {1, 0.5} };
    segment2d s2 = { {0, -1}, {0, 1} };

    const auto [result, success] = intersect_segment_segment_2D(s1, s2);
    ASSERT_TRUE(success);
    EXPECT_EQ(0.5, result.t1);
    EXPECT_EQ(0.75, result.t2);
    EXPECT_EQ(vec2d(0, 0.5), result.point);
}

TEST(IntersectSegmentSegment2DTest, OverlapingCoincident) {
    segment2d s1 = { {0,  0}, {1, 0} };
    segment2d s2 = { {0, -1}, {0, 1} };

    const auto [result, success] = intersect_segment_segment_2D(s1, s2);
    ASSERT_TRUE(success);
    EXPECT_EQ(0, result.t1);
    EXPECT_EQ(0.5, result.t2);
    EXPECT_EQ(vec2d(0, 0), result.point);
}

TEST(IntersectSegmentSegment2DTest, OverlapingPointCoincident) {
    segment2d s1 = { {1, 0}, {0, -1} };
    segment2d s2 = { {0, -1}, {0, 1} };

    const auto [result, success] = intersect_segment_segment_2D(s1, s2);
    ASSERT_TRUE(success);
    EXPECT_EQ(1, result.t1);
    EXPECT_EQ(0, result.t2);
    EXPECT_EQ(vec2d(0, -1), result.point);
}

TEST(IntersectSegmentSegment2DTest, OverlapingColinear) {
    segment2d s1 = { {0,  0}, {0, 2} };
    segment2d s2 = { {0, -1}, {0, 1} };

    const auto [result, success] = intersect_segment_segment_2D(s1, s2);
    ASSERT_TRUE(success);
    EXPECT_TRUE(
        result.point == s1[0] || result.point == s1[1] || result.point == s2[0] || result.point == s2[1]
    ) << "Expected result.point to be an endpoint";
    EXPECT_EQ(inv_lerp(s1[0], s1[1], result.point), result.t1);
    EXPECT_EQ(inv_lerp(s2[0], s2[1], result.point), result.t2);
}

TEST(IntersectSegmentSegment2DTest, NoOverlapColinear) {
    segment<2, double> s1 = { {0,  2}, {0, 3} };
    segment<2, double> s2 = { {0, -1}, {0, 1} };

    const auto [result, success] = intersect_segment_segment_2D(s1, s2);
    ASSERT_FALSE(success);
}