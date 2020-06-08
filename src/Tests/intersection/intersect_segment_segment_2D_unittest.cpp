#include <MTLib/mtlib.h>

#include <gtest/gtest.h>

using namespace mtlib;
using namespace std;

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