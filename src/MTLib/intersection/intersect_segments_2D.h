#ifndef _MTLIB_INTERSECT_SEGMENTS_2D_H_
#define _MTLIB_INTERSECT_SEGMENTS_2D_H_

#include "MTLib/algebra/vec.h"
#include "MTLib/intersection/intersect_segment_vec_2D.h"
#include "MTLib/geometry/segment.h"

#include <algorithm>
#include <array>
#include <cassert>
#include <functional>
#include <iterator>
#include <map>
#include <set>
#include <utility>
#include <vector>

namespace mtlib {
namespace {

template<typename Scalar>
struct segment_comparer;
template<typename Scalar> vec2 <Scalar> segment_comparer_point;
enum state_e {
    LEXICOGRAPHIC,
    SLOPE_BEFORE_POINT,
    SLOPE_AFTER_POINT
} segment_comparer_state = LEXICOGRAPHIC;


template<
        typename ForwardIt, typename OutputIt,
        typename Scalar = typename std::iterator_traits<ForwardIt>::value_type::scalar_type
>
void intersect_segments_2D(const ForwardIt &first, const ForwardIt &last, OutputIt d_first) {
    segment_comparer<Scalar> sweep_line_cmp;
    std::set<
        segment2<Scalar>, segment_comparer<Scalar>
    > sweep_line_state(sweep_line_cmp);

    std::map<
        vec2<Scalar>, std::set<segment2<Scalar>>
    > event_schedule;

    std::for_each(
        first, last,
        [&](auto seg) {
            event_schedule.insert({max_endpoint(seg), {}});

            const auto[it, success] = event_schedule.insert(
                    {min_endpoint(seg), {seg}}
            );
            if (!success)
                it->second.insert(seg);
        }
    );

    while (!event_schedule.empty()) {
        auto event = event_schedule.begin();
        const auto point = event->first;

        std::set<segment2<Scalar>> right;
        std::set<segment2<Scalar>> contains;
        std::set<segment2<Scalar>> &left = event->second;

        segment_comparer_state = SLOPE_BEFORE_POINT;
        segment_comparer_point<Scalar> = point;

        const auto segments_touching_point = sweep_line_state.equal_range(point);
        for (auto it = segments_touching_point.first; it != segments_touching_point.second; ++it) {
            if (max_endpoint(*it) == point)
                right.insert(*it);
            else
                contains.insert(*it);
        }

        // if | right U contains U left | > 1, report event

        segment_comparer_state = SLOPE_AFTER_POINT;
        std::set<segment2<Scalar>, segment_comparer<Scalar>> cl(contains.begin(), contains.end(), sweep_line_cmp);
        for (auto seg : left)
            cl.insert(seg);

        std::set<segment2<Scalar>> rcl(cl.begin(), cl.end());
        for (auto seg : right)
            rcl.insert(seg);

        // report event
        if (rcl.size() > 1) {
            std::vector<segment2<Scalar>> out_segments(rcl.begin(), rcl.end());
            d_first = std::make_pair(point, out_segments);
        }

        segment_comparer_state = SLOPE_BEFORE_POINT;
        for (auto seg : right)
            sweep_line_state.erase(seg);

        for (auto seg : contains)
            sweep_line_state.erase(seg);


        {   // look for new intersection events
            auto add_if_intersecting = [&](const segment2<Scalar> &lhs, const segment2<Scalar> &rhs) {
                if (lhs != rhs) {
                    const auto[result, success] = intersect_segment_segment_2D(lhs, rhs);
                    if (success && result.point > point)
                        event_schedule.insert({result.point, {}});
                }
            };

            if (sweep_line_state.size() > 1) {
                const auto[lower_segment, upper_segment] = sweep_line_state.equal_range(point);
                assert(lower_segment == upper_segment || upper_segment == sweep_line_state.end());
                // if lower and upper are not equal
                //  lower_segment contains point
                //      - lower_segment should have been removed from sweep_line and added to contains

                if (lower_segment == sweep_line_state.end()) {
                    if (!cl.empty())
                        add_if_intersecting(*(--cl.end()), *std::prev(lower_segment));
                }
                else if (upper_segment == sweep_line_state.begin()) {
                    if (!cl.empty())
                        add_if_intersecting(*upper_segment, *cl.begin());
                }
                else {
                    if (cl.empty()) {
                        add_if_intersecting(*std::prev(lower_segment), *upper_segment);
                    }
                    else {
                        add_if_intersecting(*std::prev(lower_segment), *cl.begin());
                        add_if_intersecting(*std::prev(cl.end()), *upper_segment);
                    }
                }
            }
            else if (sweep_line_state.size() == 1 && !cl.empty()) {
                const auto &sweep_segment = *sweep_line_state.begin();
                add_if_intersecting(sweep_segment, *cl.begin());
                add_if_intersecting(*(--cl.end()), sweep_segment);
            }
        }

        segment_comparer_state = SLOPE_AFTER_POINT;
        for (auto seg : cl)
            sweep_line_state.insert(seg);

        event_schedule.erase(event);
    }
}

template<typename Scalar>
struct segment_comparer {
    using is_transparent = vec2<Scalar>;

    bool operator()(const segment2 <Scalar> &lhs, const segment2 <Scalar> &rhs) const {
        switch (segment_comparer_state) {
            case LEXICOGRAPHIC:
                return lexicographic(lhs, rhs);
            case SLOPE_BEFORE_POINT:
                return slope_before_point(lhs, rhs);
            case SLOPE_AFTER_POINT:
                return slope_after_point(lhs, rhs);
        }

        return false;
    }

    bool operator()(const segment2 <Scalar> &lhs, const vec2 <Scalar> &rhs) const {
        if (lhs[0][0] == lhs[1][0])
            return !overlap_segment_vec_2D(lhs, rhs);   // two failures
        return evaluate_at_x_2D(lhs, rhs[0]) < rhs[1];
    }

    bool operator()(const vec2 <Scalar> &lhs, const segment2 <Scalar> &rhs) const {
        if (rhs[0][0] == rhs[1][0])
            return !overlap_segment_vec_2D(rhs, lhs);   // make an equals
        return lhs[1] < evaluate_at_x_2D(rhs, lhs[0]);
    }

private:
    constexpr bool lexicographic(const segment2 <Scalar> &lhs, const segment2 <Scalar> &rhs) const {
        return lhs < rhs;
    }

    constexpr bool slope_before_point(const segment2 <Scalar> &lhs, const segment2 <Scalar> &rhs) const {
        bool overlap_lhs = overlap_segment_vec_2D(lhs, segment_comparer_point<Scalar>);
        bool overlap_rhs = overlap_segment_vec_2D(rhs, segment_comparer_point<Scalar>);
        const auto[slope_lhs, slope_lhs_exists] = slope_2D(lhs);
        const auto[slope_rhs, slope_rhs_exists] = slope_2D(rhs);

        if (overlap_lhs && overlap_rhs) {
            if (slope_lhs_exists && slope_rhs_exists) {
                return slope_lhs > slope_rhs || (slope_lhs == slope_rhs && lhs < rhs);
            } else if (!slope_lhs_exists && !slope_rhs_exists) {
                return lhs < rhs;
            }

            return slope_rhs_exists;
        }

        if (slope_lhs_exists && slope_rhs_exists) {
            return evaluate_at_x_2D(lhs, segment_comparer_point<Scalar>[0]) <
                   evaluate_at_x_2D(rhs, segment_comparer_point<Scalar>[0]);
        }
        else if (!slope_lhs_exists && !slope_rhs_exists) {
            return max_on_dim(lhs, 1) < max_on_dim(rhs, 1);
        }
        else if (slope_lhs_exists) {
            return evaluate_at_x_2D(lhs, segment_comparer_point<Scalar>[0]) < segment_comparer_point<Scalar>[1];
        }

        // slope_rhs_exists
        return segment_comparer_point<Scalar>[1] < evaluate_at_x_2D(rhs, segment_comparer_point<Scalar>[0]);
    }

    constexpr bool slope_after_point(const segment2 <Scalar> &lhs, const segment2 <Scalar> &rhs) const {
        bool overlap_lhs = overlap_segment_vec_2D(lhs, segment_comparer_point<Scalar>);
        bool overlap_rhs = overlap_segment_vec_2D(rhs, segment_comparer_point<Scalar>);
        const auto[slope_lhs, slope_lhs_exists] = slope_2D(lhs);
        const auto[slope_rhs, slope_rhs_exists] = slope_2D(rhs);

        if (overlap_lhs && overlap_rhs) {
            if (slope_lhs_exists && slope_rhs_exists) {
                return slope_lhs < slope_rhs || (slope_lhs == slope_rhs && lhs > rhs);
            } else if (!slope_lhs_exists && !slope_rhs_exists) {
                return lhs > rhs;
            }

            return slope_lhs_exists;
        }

        if (slope_lhs_exists && slope_rhs_exists) {
            return evaluate_at_x_2D(lhs, segment_comparer_point<Scalar>[0]) <
                   evaluate_at_x_2D(rhs, segment_comparer_point<Scalar>[0]);
        }
        else if (!slope_lhs_exists && !slope_rhs_exists) {
            return max_on_dim(lhs, 1) < max_on_dim(rhs, 1);
        }
        else if (slope_lhs_exists) {
            return evaluate_at_x_2D(lhs, segment_comparer_point<Scalar>[0]) < segment_comparer_point<Scalar>[1];
        }

        // slope_rhs_exists
        return segment_comparer_point<Scalar>[1] < evaluate_at_x_2D(rhs, segment_comparer_point<Scalar>[0]);
    }
};

}
}   // namespace mtlib

#endif // _MTLIB_INTERSECT_SEGMENTS_2D_H_