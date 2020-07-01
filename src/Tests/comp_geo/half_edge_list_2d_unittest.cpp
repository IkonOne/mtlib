#include <MTLib/mtlib.h>

#include <gtest/gtest.h>

#include <iostream>

using namespace mtlib;
using namespace std;

class HalfEdgeList2dTestFixture : public ::testing::Test {
protected:
    half_edge_list_2d<double> el;

    auto create_triangle() {
        vec2d v1(0, 0), v2(1, 0), v3(1, 1);
        el.add_edge(v1, v2);
        el.add_edge(v2, v3);
        el.add_edge(v3, v1);

        return std::make_tuple(
            &*(el.vertices().begin()),
            &*(++el.vertices().begin()),
            &*(--el.vertices().end())
        );
    }
};

TEST_F(HalfEdgeList2dTestFixture, AddEdgeTwins) {
    el.add_edge(vec2d(0, 0), vec2d(1, 1));

    ASSERT_EQ(2, el.vertices().size());
    ASSERT_EQ(2, el.half_edges().size());

    const auto& e1 = *el.half_edges().begin();
    const auto& e2 = *(++el.half_edges().begin());

    EXPECT_EQ(e1.twin, &e2);
    EXPECT_EQ(e2.twin, &e1);
}

TEST_F(HalfEdgeList2dTestFixture, AddDuplicateEdgeFails) {
    vec2d v1(0, 0), v2(1, 1);

    el.add_edge(v1, v2);
    const auto [he_it, duplicate_added] = el.add_edge(v1, v2);

    ASSERT_FALSE(duplicate_added);
}

TEST_F(HalfEdgeList2dTestFixture, AddPathHalfEdgesIncident) {
    vec2d v1(0, 0), v2(1, 0), v3(1, 1);
    el.add_edge(v1, v2);
    el.add_edge(v2, v3);

    /*
     *            v3
     *           ^ +
     *           | |
     *           | |
     *           + v
     *   v1+----> v2
     *     <----+
     */

    const auto& vert1 = *el.vertices().begin();
    const auto& vert2 = *(++el.vertices().begin());
    const auto& vert3 = *(--el.vertices().end());

    EXPECT_EQ(vert1.incident->destination(), &vert2);
    EXPECT_EQ(vert1.incident->twin, vert2.incident);
    EXPECT_EQ(vert1.incident->next->twin, vert3.incident);

    EXPECT_EQ(vert3.incident->next->twin, vert1.incident);

    ASSERT_NE(vert2.incident->prev, vert2.incident->twin);
    ASSERT_EQ(vert2.incident->next, vert2.incident->twin);
    EXPECT_EQ(vert2.incident->prev->twin, vert2.incident->twin->next);
}

TEST_F(HalfEdgeList2dTestFixture, TriangleVerticesIncidentHasMinPseudoAngleDestination) {
    const auto [v1, v2, v3] = create_triangle();

    ASSERT_EQ(3, el.vertices().size());

    EXPECT_EQ(v1->incident->destination(), v2);
    EXPECT_EQ(v2->incident->destination(), v1);
    EXPECT_EQ(v3->incident->destination(), v1);
}

TEST_F(HalfEdgeList2dTestFixture, ExteriorTriangleIsCW) {
    const auto [v1, v2, v3] = create_triangle();

    EXPECT_EQ(v2->incident->destination(), v1);
    EXPECT_EQ(v2->incident->next->destination(), v3);
    EXPECT_EQ(v2->incident->next->next->destination(), v2);

    EXPECT_EQ(v2->incident->prev->destination(), v2);
    EXPECT_EQ(v2->incident->prev->prev->destination(), v3);
}

TEST_F(HalfEdgeList2dTestFixture, AddTriangleFaces) {
    const auto [v1, v2, v3] = create_triangle();

    FAIL();
}

class HalfEdgeList2dVertexIncidentIteratorFixture : public ::testing::Test {
protected:
    using el = half_edge_list_2d<double>;
    el::vertex origin = vec2d(0, 0);
    el::vertex dest1 = vec2d(0, 0);
    el::vertex dest2 = vec2d(0, 0);
    el::vertex dest3 = vec2d(0, 0);
    el::half_edge he1;
    el::half_edge he2;
    el::half_edge he3;
    el::half_edge he1_twin;
    el::half_edge he2_twin;
    el::half_edge he3_twin;

    void SetUp() override {
        origin.incident = &he1;

        he1.origin      = &origin;      he1.twin        = &he1_twin;
        he1.prev        = &he2_twin;    he1.next        = &he1_twin;
        he1_twin.origin = &dest1;       he1_twin.twin   = &he1;
        he1_twin.prev   = &he1;         he1_twin.next   = &he3;

        he2.origin      = &origin;      he2.twin        = &he2_twin;
        he2.prev        = &he3_twin;    he2.next        = &he2_twin;
        he2_twin.origin = &dest2;       he2_twin.twin   = &he2;
        he2_twin.prev   = &he2;         he2_twin.next   = &he1;

        he3.origin      = &origin;      he3.twin        = &he3_twin;
        he3.prev        = &he1_twin;    he3.next        = &he3_twin;
        he3_twin.origin = &dest3;       he3_twin.twin   = &he3;
        he3_twin.prev   = &he3;         he3_twin.next   = &he2;
    }
};

TEST_F(HalfEdgeList2dVertexIncidentIteratorFixture, Bidirectional) {
    auto curr = origin.begin();
    ASSERT_EQ(he1, *curr++);
    ASSERT_EQ(he2, *curr++);
    ASSERT_EQ(he3, *curr++);
    ASSERT_EQ(origin.end(), curr);
    ASSERT_EQ(he3, *(--curr));
    ASSERT_EQ(he2, *(--curr));
    ASSERT_EQ(he1, *(--curr));
    ASSERT_EQ(origin.begin(), curr);
}

TEST_F(HalfEdgeList2dVertexIncidentIteratorFixture, ReverseBidirectional) {
    auto curr = origin.rbegin();
    ASSERT_EQ(he3, *curr++);
    ASSERT_EQ(he2, *curr++);
    ASSERT_EQ(he1, *curr++);
    ASSERT_EQ(origin.rend(), curr);
    ASSERT_EQ(he1, *(--curr));
    ASSERT_EQ(he2, *(--curr));
    ASSERT_EQ(he3, *(--curr));
    ASSERT_EQ(origin.rbegin(), curr);
}