//
// Created by Erin M Gunn on 7/23/20.
//

#ifndef MTLIB_DCEL_H
#define MTLIB_DCEL_H

#include "MTLib/algebra/vec.h"

#include <algorithm>
#include <functional> // reference_wrapper
#include <cstddef> // size_t
#include <list> // using lists since removal is the only time references are invalidated

namespace mtlib {
namespace ds {

template<typename Traits> class dcel;
struct dcel_list_Traits;
using dcel_list = dcel<dcel_list_Traits>;

template<typename Traits>
struct dcel_vertex {
    typename Traits::half_edge* incident = nullptr;
};

template<typename Traits>
struct dcel_half_edge {
    typename Traits::vertex* origin = nullptr;
    typename Traits::half_edge* twin = nullptr;
    typename Traits::half_edge* next = nullptr;
    typename Traits::half_edge* prev = nullptr;
    typename Traits::face* face = nullptr;
};

template<typename Traits>
struct dcel_face {
    typename Traits::half_edge* incident = nullptr;
};

struct dcel_list_Traits {
    using vertex = dcel_vertex<dcel_list_Traits>;
    using vertex_container = std::list<vertex>;
    using vertices_iterator = vertex_container::iterator;
    using vertices_reverse_iterator = vertex_container::reverse_iterator;

    using half_edge = dcel_half_edge<dcel_list_Traits>;
    using half_edge_container = std::list<half_edge>;
    using half_edges_iterator = half_edge_container::iterator;
    using half_edges_reverse_iterator = half_edge_container::reverse_iterator;

    using face = dcel_face<dcel_list_Traits>;
    using face_container = std::list<face>;
    using faces_iterator = face_container::iterator;
    using faces_reverse_iterator = face_container::reverse_iterator;
};

template<typename Traits>
class dcel {
public:
    using vertex = typename Traits::vertex;
    using vertex_container = typename Traits::vertex_container;
    using vertices_iterator = typename Traits::vertices_iterator;
    using vertices_reverse_iterator = typename Traits::vertices_reverse_iterator;

    using half_edge = typename Traits::half_edge;
    using half_edge_container = typename Traits::half_edge_container;
    using half_edges_iterator = typename Traits::half_edges_iterator;
    using half_edges_reverse_iterator = typename Traits::half_edges_reverse_iterator;

    using face = typename Traits::face;
    using face_container = typename Traits::face_container;
    using faces_iterator = typename Traits::faces_iterator;
    using faces_reverse_iterator = typename Traits::faces_reverse_iterator;

public:
    static constexpr vertex* null_vertex = nullptr;
    static constexpr half_edge* null_half_edge = nullptr;
    static constexpr face* null_face = nullptr;

    constexpr typename vertex_container::size_type vertices_size() const { return vertices_.size(); }
    constexpr bool vertices_empty() const { return vertices_.empty(); }

    vertex* create_vertex() {
        vertices_.emplace_back();
        auto& v = vertices_.back();
        v.incident = null_half_edge;
        return &v;
    }

    bool is_consistent(const vertex* handle) {
        return handle->incident != null_half_edge;
    }

    constexpr vertices_iterator vertices_begin() { return vertices_.begin(); }
    constexpr vertices_iterator vertices_end() { return vertices_.end(); }
    constexpr vertices_reverse_iterator vertices_rbegin() { return vertices_.rbegin(); }
    constexpr vertices_reverse_iterator vertices_rend() { return vertices_.rend(); }

    constexpr typename half_edge_container::size_type half_edges_size() const { return half_edges_.size(); }
    constexpr bool half_edges_empty() const { return half_edges_.empty(); }

    half_edge* create_half_edge() {
        half_edges_.emplace_back();
        auto& h = half_edges_.back();
        h.origin = null_vertex;
        h.twin = null_half_edge;
        h.prev = null_half_edge;
        h.next = null_half_edge;
        h.face = null_face;
        return &h;
    }

    constexpr bool is_consistent(const half_edge* handle) {
        return handle->origin != null_vertex &&
            handle->twin != null_half_edge &&
            handle->prev != null_half_edge &&
            handle->next != null_half_edge &&
            handle->face != null_face;
    }

    constexpr half_edges_iterator half_edges_begin() { return half_edges_.begin(); }
    constexpr half_edges_iterator half_edges_end() { return half_edges_.end(); }
    constexpr half_edges_reverse_iterator half_edges_rbegin() { return half_edges_.rbegin(); }
    constexpr half_edges_reverse_iterator half_edges_rend() { return half_edges_.rend(); }

    constexpr typename face_container::size_type faces_size() const { return faces_.size(); }
    constexpr bool faces_empty() const { return faces_.empty(); }

    face* create_face() {
        faces_.emplace_back();
        auto& f = faces_.back();
        f.incident = null_half_edge;
        return &f;
    }

    constexpr bool is_consistent(const face* handle) {
        return handle->incident != null_half_edge;
    }

    constexpr faces_iterator faces_begin() { return faces_.begin(); }
    constexpr faces_iterator faces_end() { return faces_.end(); }
    constexpr faces_reverse_iterator faces_rbegin() { return faces_.rbegin(); }
    constexpr faces_reverse_iterator faces_rend() { return faces_.rend(); }

private:
    vertex_container vertices_;
    half_edge_container half_edges_;
    face_container faces_;
};

} // ds
} // mtlib

#endif //MTLIB_DCEL_H
