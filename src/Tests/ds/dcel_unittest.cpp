//
// Created by Erin M Gunn on 7/24/20.
//

#include <MTLib/mtlib.h>

#include <gtest/gtest.h>

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