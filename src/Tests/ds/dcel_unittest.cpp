//
// Created by Erin M Gunn on 7/24/20.
//

#include <MTLib/mtlib.h>

#include <gtest/gtest.h>

#include <array>
#include <functional>

using namespace mtlib;
using namespace mtlib::ds;
using namespace std;

TEST(DCELTest, Instantiation) {
    dcel_list d;
    ASSERT_TRUE(d.vertices_empty());
    ASSERT_TRUE(d.half_edges_empty());
    ASSERT_TRUE(d.faces_empty());

    ASSERT_EQ(0, d.vertices_size());
    ASSERT_EQ(0, d.half_edges_size());
    ASSERT_EQ(0, d.faces_size());
}

TEST(DCELTest, CreateVertex) {
    dcel_list d;
    auto v = d.create_vertex();

    ASSERT_FALSE(d.is_consistent(v));
    ASSERT_EQ(1, d.vertices_size());
    ASSERT_EQ(d.null_half_edge, v->incident);
}

TEST(DCELTest, VeretexIsConsistent) {
    dcel_list d;
    auto v = d.create_vertex();
    v->incident = d.create_half_edge();

    ASSERT_TRUE(d.is_consistent(v));
}

TEST(DCELTest, CreateHalfEdge) {
    dcel_list d;
    auto h = d.create_half_edge();

    ASSERT_FALSE(d.is_consistent(h));
    ASSERT_EQ(1, d.half_edges_size());
    ASSERT_EQ(d.null_vertex, h->origin);
    ASSERT_EQ(d.null_half_edge, h->twin);
    ASSERT_EQ(d.null_half_edge, h->next);
    ASSERT_EQ(d.null_half_edge, h->prev);
    ASSERT_EQ(d.null_face, h->face);
}

TEST(DCELTest, HalfEdgeIsConsistent) {
    dcel_list d;
    auto h = d.create_half_edge();
    h->origin = d.create_vertex();
    h->twin = h->prev = h->next = d.create_half_edge();
    h->face = d.create_face();

    ASSERT_TRUE(d.is_consistent(h));
}

TEST(DCELTest, CreateFace) {
    dcel_list d;
    auto f = d.create_face();

    ASSERT_FALSE(d.is_consistent(f));
    ASSERT_EQ(1, d.faces_size());
    ASSERT_EQ(d.null_half_edge, f->incident);
}

TEST(DCELTest, FaceIsConsistent) {
    dcel_list d;
    auto f = d.create_face();
    f->incident = d.create_half_edge();

    ASSERT_TRUE(d.is_consistent(f));
}

class DCELIteratorsTest : public ::testing::Test {
protected:
    dcel_list d;
    std::array<dcel_list::vertex*, 3> vertices;
    std::array<dcel_list::half_edge*, 3> inner_half_edges;
    std::array<dcel_list::half_edge*, 3> outer_half_edges;
    dcel_list::face* inner_face;

    void createTriangle() {
        auto v0 = vertices[0] = d.create_vertex();
        auto v1 = vertices[1] = d.create_vertex();
        auto v2 = vertices[2] = d.create_vertex();

        auto h0i = inner_half_edges[0] = d.create_half_edge();
        auto h1i = inner_half_edges[1] = d.create_half_edge();
        auto h2i = inner_half_edges[2] = d.create_half_edge();
        auto h0o = outer_half_edges[0] = d.create_half_edge();
        auto h1o = outer_half_edges[1] = d.create_half_edge();
        auto h2o = outer_half_edges[2] = d.create_half_edge();

        inner_face = d.create_face();

        v0->incident = h0i;
        v1->incident = h1i;
        v2->incident = h2i;

        h0i->origin = v0; h0o->origin = v0;
        h1i->origin = v1; h1o->origin = v1;
        h2i->origin = v2; h2o->origin = v2;

        h0i->twin = h1o; h0o->twin = h2i;
        h1i->twin = h2o; h1o->twin = h0i;
        h2i->twin = h0o; h2o->twin = h1i;

        h0i->prev = h2i; h0o->next = h2o;
        h1i->prev = h0i; h1o->next = h0o;
        h2i->prev = h1i; h2o->next = h1o;

        h0i->next = h1i; h0o->prev = h1o;
        h1i->next = h2i; h1o->prev = h2o;
        h2i->next = h0i; h2o->prev = h0o;

        h0i->face = h1i->face = h2i->face = inner_face;
    }

    void SetUp() override {
        createTriangle();
    }
};

TEST_F(DCELIteratorsTest, HalfEdgeLoopIterator) {
    auto he1 = inner_half_edges[0];
    auto it = d.half_edge_loop_begin(he1);
    auto it_begin = d.half_edge_loop_begin(he1);
    auto it_end = d.half_edge_loop_end(he1);

    EXPECT_EQ(it_begin, it);
    EXPECT_EQ(*inner_half_edges[0], *(it));
    EXPECT_EQ(*inner_half_edges[1], *(++it));
    EXPECT_EQ(*inner_half_edges[2], *(++it));
    EXPECT_EQ(it_end, ++it);
    EXPECT_NE(it_begin, it);
}

TEST_F(DCELIteratorsTest, HalfEdgeLoopReverseIterator) {
    auto he1 = outer_half_edges[0];
    auto it = d.half_edge_loop_rbegin(he1);
    auto it_begin = d.half_edge_loop_rbegin(he1);
    auto it_end = d.half_edge_loop_rend(he1);

    EXPECT_EQ(it_begin, it);
    EXPECT_EQ(*outer_half_edges[1], *(it));
    EXPECT_EQ(*outer_half_edges[2], *(++it));
    EXPECT_EQ(*outer_half_edges[0], *(++it));
    EXPECT_EQ(it_end, ++it);
    EXPECT_NE(it_begin, it);
}