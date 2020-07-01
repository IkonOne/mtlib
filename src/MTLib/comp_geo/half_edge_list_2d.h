#ifndef _MTLIB_HALF_EDGE_LIST_2D_H_
#define _MTLIB_HALF_EDGE_LIST_2D_H_

#include "MTLib/algebra/common.h"
#include "MTLib/algebra/linalg.h"
#include "MTLib/algebra/vec.h"


#include <cassert>
#include <functional>
#include <list>
#include <memory>
#include <set>
#include <utility>

namespace mtlib {

/**
 * All managed std::containers are std::set<T>.  This enables us to use pointers to elements in the sets
 * in our implementation because sets do not invalidate iterators or references when modifying the container.
 * (except in obvious cases when removing)
 *
 * @tparam Scalar
 */

template <typename Scalar>
class half_edge_list_2d {
public:
    struct vertex;
    struct half_edge;
    struct face;

    using vertex_container = std::set<vertex>;
    using half_edge_container = std::set<half_edge>;
    using face_container = std::set<face>;

public:
    half_edge_list_2d() : boundary_(new face()) {
        faces_.insert(boundary());
    }

// Iterators
public:
    /**
     * An bidirectional iterator that visits all of the half_edges incident to a vertex where '++' is ccw order.
     */
    struct vertex_incident_iterator;

    vertex_incident_iterator vertex_incident_begin(const vertex& vert, const half_edge* first = nullptr) const {
        if (first)
            return vertex_incident_iterator(vert, first);
        return vertex_incident_iterator(vert);
    }

    vertex_incident_iterator vertex_incident_end(const vertex& vert, const half_edge* first = nullptr) const {
        if (first)
            return vertex_incident_iterator(vert, first, true);
        return vertex_incident_iterator(vert, true);
    }


// Accessors
public:
    const vertex_container& vertices() const { return vertices_; }
    const half_edge_container& half_edges() const { return half_edges_; }
    const face_container& faces() const { return faces_; }
    face& boundary() const { return *boundary_; }

// Modifiers
public:
    std::pair<const half_edge&, bool>
    add_edge(const vec2<Scalar>& lhs, const vec2<Scalar>& rhs)
    {
        auto [origin_vert_it, origin_vert_created] = vertices_.emplace(lhs);
        vertex* origin_vert = const_cast<vertex*>(&*origin_vert_it);

        // don't do anything else if this edge already exists
        // FIXME: This is low percentage in practice and might be better handled in the insertion logic below
        // O(i) : i is # half_edges incident to origin/lhs
        // vs doing it in insertion logic
        if (!origin_vert_created) {
            for (auto it = origin_vert->begin(); it != origin_vert->end(); ++it) {
                if (*(it->destination()) == rhs) {
                    return std::make_pair(*it, false);
                }
            }
        }

        auto [dest_vert_it, dest_vert_created] = vertices_.emplace(rhs);
        vertex* dest_vert = const_cast<vertex*>(&*dest_vert_it);

        half_edge scratch_he(dest_vert);  // required to correctly sort the half_edge in half_edges_
        auto [origin_he_it, origin_he_created] = half_edges_.emplace(origin_vert, &scratch_he);
        half_edge* origin_he = const_cast<half_edge*>(&*origin_he_it);

        auto [dest_he_it, dest_he_created] = half_edges_.emplace(dest_vert, origin_he);
        half_edge* dest_he = const_cast<half_edge*>(&*dest_he_it);

        origin_he->twin = dest_he;

        // FIXME: Given we are sorting via pseudo-angle, we could get O(lgn) bounds using an std::set/bbst instead of O(n)
        // Is that worth the extra storage?  Depends on usage?
        auto find_half_edge_insertion = [](const vertex& origin, const half_edge& he) {
            auto angle_he = pseudo_angle(*he.destination() - origin);
            auto curr_it = origin.begin();

            while (curr_it != origin.end() && pseudo_angle(*curr_it->destination() - origin) < angle_he)
                ++curr_it;

            half_edge* next = (curr_it == origin.end() ? origin.incident : &*curr_it);
            half_edge* prev = &*--curr_it;
            return std::make_pair(prev, next);
        };

        if (origin_vert_created) {
            origin_vert->incident = origin_he;
            origin_he->prev = dest_he;
            dest_he->next = origin_he;
        }
        else {
            auto [origin_prev_incident, origin_next_incident] = find_half_edge_insertion(*origin_vert, *origin_he);
            origin_he->prev = origin_next_incident->twin;
            origin_he->prev->next = origin_he;
            dest_he->next = origin_prev_incident;
            origin_prev_incident->prev = dest_he;

            if ( origin_next_incident == origin_vert->incident &&
                pseudo_angle(*origin_he->destination() - *origin_vert) < pseudo_angle(*origin_next_incident->destination() - *origin_vert) )
            {
                origin_vert->incident = origin_he;
            }
        }

        if (dest_vert_created) {
            dest_vert->incident = dest_he;
            dest_he->prev = origin_he;
            origin_he->next = dest_he;
        }
        else {
            auto [dest_prev_incident, dest_next_incident] = find_half_edge_insertion(*dest_vert, *dest_he);
            dest_he->prev = dest_next_incident->twin;
            dest_he->prev->next = dest_he;
            origin_he->next = dest_prev_incident;
            dest_prev_incident->prev = origin_he;

            if ( dest_next_incident == dest_vert->incident &&
                pseudo_angle(*dest_he->destination() - *dest_vert) < pseudo_angle(*dest_next_incident->destination() - *dest_vert) )
            {
                dest_vert->incident = dest_he;
            }
        }

        // TODO: Add the right faces.  Chpt. 6 is on trapezoidal maps which will allow this in lgn time

        return std::make_pair(*origin_he, true);
    }

// Members
private:
    vertex_container vertices_;
    half_edge_container half_edges_;
    face_container faces_;
    std::unique_ptr<face> boundary_;

public:
    struct vertex : public vec2<Scalar> {
        vertex(const vec2<Scalar>& p) : vec2<Scalar>(p)
        {}

        vertex_incident_iterator begin() const {
            return vertex_incident_iterator(this);
        }

        vertex_incident_iterator end() const {
            return vertex_incident_iterator(this, true);
        }

        std::reverse_iterator<vertex_incident_iterator> rbegin() const {
            return std::make_reverse_iterator(end());
        }

        std::reverse_iterator<vertex_incident_iterator> rend() const {
            return std::make_reverse_iterator(begin());
        }

        half_edge* incident;
    };

    struct half_edge {
        vertex* origin;
        half_edge* twin;
        half_edge* prev;
        half_edge* next;
        face* incident;

        half_edge(
            vertex* origin_v = nullptr, half_edge* twin_he = nullptr, half_edge* prev_he = nullptr,
            half_edge* next_he = nullptr, face* incident_f = nullptr
        ) : origin(origin_v), twin(twin_he), prev(prev_he), next(next_he), incident(incident_f) {}

        inline vertex* destination() const { return twin->origin; }

        friend bool operator==(const half_edge& lhs, const half_edge& rhs) {
            return *lhs.origin == *rhs.origin && *lhs.twin->origin == *rhs.twin->origin;
        }

        friend bool operator!=(const half_edge& lhs, const half_edge& rhs) {
            return *lhs.origin != *rhs.origin || *lhs.twin->origin != *rhs.twin->origin;
        }

        friend bool operator<=(const half_edge& lhs, const half_edge& rhs) {
            return *lhs.origin < *rhs.origin ||
                (*lhs.origin == *rhs.origin && *lhs.twin->origin <= *rhs.twin->origin);
        }

        friend bool operator<(const half_edge& lhs, const half_edge& rhs) {
            return *lhs.origin < *rhs.origin ||
                (*lhs.origin == *rhs.origin && *lhs.twin->origin < *rhs.twin->origin);
        }

        friend bool operator>=(const half_edge& lhs, const half_edge& rhs) {
            return *lhs.origin > *rhs.origin ||
                (*lhs.origin == *rhs.origin && *lhs.twin->origin >= *rhs.twin->origin);
        }

        friend bool operator>(const half_edge& lhs, const half_edge& rhs) {
            return *lhs.origin > *rhs.origin ||
                (*lhs.origin == *rhs.origin && *lhs.twin->origin > *rhs.twin->origin);
        }
    };

    struct face {
        half_edge* outer;
        half_edge_container inner;

        // The only face that will ever have a null outer is the boundary.
        friend bool operator==(const face& lhs, const face& rhs) { return lhs.outer == rhs.outer; }
        friend bool operator!=(const face& lhs, const face& rhs) { return lhs.outer != rhs.outer; }

        friend bool operator<(const face& lhs, const face& rhs) {
            if (rhs.outer == nullptr) return false;
            return lhs.outer == nullptr || *lhs.outer < *rhs.outer;
        }

        friend bool operator<=(const face& lhs, const face& rhs) {
            if (rhs.outer == nullptr) return false;
            return lhs.outer == nullptr || *lhs.outer <= *rhs.outer;
        }

        friend bool operator>(const face& lhs, const face& rhs) {
            if (rhs.outer == nullptr) return false;
            return lhs.outer != nullptr && *lhs.outer > *rhs.outer;
        }

        friend bool operator>=(const face& lhs, const face& rhs) {
            if (rhs.outer == nullptr) return false;
            return lhs.outer != nullptr && *lhs.outer >= *rhs.outer;
        }
    };

public:
    struct vertex_incident_iterator {
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = half_edge;
        using difference_type = std::size_t;
        using pointer = value_type*;
        using reference = value_type&;

        vertex_incident_iterator() = delete;
        ~vertex_incident_iterator() = default;
        vertex_incident_iterator& operator=(const vertex_incident_iterator&) = default;

        vertex_incident_iterator(const vertex* vert, bool is_end = false)
            : vert_(vert),
                curr_(is_end ? nullptr : const_cast<half_edge*>(vert->incident))
        {}

        vertex_incident_iterator& operator++() {
            if (curr_ != nullptr)
                curr_ = curr_->prev->twin != vert_->incident ? curr_->prev->twin : nullptr;
            return *this;
        }

        vertex_incident_iterator operator++(int) {
            auto temp = *this;
            ++*this;
            return temp;
        }

        vertex_incident_iterator& operator--() {
            if (curr_ != vert_->incident)
                curr_ = curr_ == nullptr ? vert_->incident->twin->next : curr_->twin->next;
            return *this;
        }

        vertex_incident_iterator operator--(int) {
            auto temp = *this;
            --*this;
            return temp;
        }

        half_edge& operator*() const {
            return *curr_;
        }

        half_edge* operator->() const {
            return curr_;
        }

        friend bool operator==(const vertex_incident_iterator& lhs, const vertex_incident_iterator& rhs) {
            return lhs.curr_ == rhs.curr_;
        }

        friend bool operator!=(const vertex_incident_iterator& lhs, const vertex_incident_iterator& rhs) {
            return lhs.curr_ != rhs.curr_;
        }

    private:
        const vertex* vert_;
        half_edge* curr_;
    };
};

}//mtlib

#endif //_MTLIB_HALF_EDGE_LIST_2D_H
