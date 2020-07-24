# Deprecated
See decl-api.md

# 2D Doubly Connected Edge List API Design Doc

Note: This is a living (probably dead) document that is meant as a guide, not a contract.

## Requirements

Let's just stick with 2d for now.

Can a single DCEL facilitate both 2d and 3d?
- Thinking of boolean operations, in 2d, we only care if edges intersect.  In 3d we care about edges and faces.
- How do you have a face in 3d if all vertices are not coplanar?  This effects inner and outer faces.

It would be nice to be able to bake/freeze the DCEL so that we can get O(1) access to all the things.  (what things???)
This could be accomplished with a builder pattern with facade/flyweight for data access.

### TODO
- [x] What does end() mean???
  - Figure it out.
- [x] Can nested classes (vertex, incident, face) be flyweights so we can have different back end implementations???
  - Sure, but refactor after initial implementation if needed.
- [x] Is there a predefined order (e.g. vertex can be ordered lexicographically on it's vector)
  - vertices
    - lexicographic order makes sense
      - Easy to use with sweep line algorithms
      - Ensures no duplicates (set< vertex > )
  - half-edges
    - order lexicographically by (origin, destination)
      - edges with destinations further to (-x,-y) come first
    - order by origin lexicographically, then by ccw around origin
      - ccw around origin adds no additional assumptions
  - faces
    - hash
- [x] Does it even make sense to design the api to be std::container compatible?  What algorithms would work on a "containerized" dcel?
  - First glance, yes.  Stay the course.
    - all_of, any_of, none_of, for_each, count_if, find_if, transform, min/max_element

### Data

vertex
  - Has a:
    - Half Edge
    - Point/Vector 2d or 3d
    - data : VertexData
  - Is:
    - hashable

Half Edge
  - Has a:
    - Origin : vertex
    - Twin : Half Edge
    - Prev : Half Edge
    - Next : Half Edge
    - Face
    - data : HalfEdgeData
  - Is:
    - hashable

Face  
  - Has a:
    - Outer : Half Edge
    - Inner : List of Half Edges
    - data : FaceData
  - Is:
    - hashable

Doubly Connected Edge List
  - Has a:
    - bounding face : Face that is the outer face of all subdivisions (wording, "outer face")
    - vertices : reference to sorted container of vertices
    - faces : reference to sorted container of faces

---
### Operations

- [x] Erase vertex
  - Adding a single vertex would be hard without an opposing vertex because what would it's half edges twin be?
    - Unless we are adding the vertex on top of an existing edge...
    - ??? Maybe an "insert_vertex" makes sense
- [x] Add/Remove Edge
  - Should automatically handle creation and deletion of faces.
- [x] visit half edges of half edge loop
  - Inner and Outer half edge loops of a face can be traversed.
  - From this consumer can get:
    - All vertices of the half edge loop
    - All inner and/or outer faces of the half edge loop incident to the edges
- [x] visit half edges of vertex
  - From this the consumer can get:
    - All half edges incident to the vertex
    - All faces incident to the vertex
- [x] get vertex at position
- [x] visit all half edges
- [x] visit all vertices
- [x] visit all faces

---
### Iterators

[x] each vertex
- lexicographic order

[x] half_edges which have origin == vertex
- ccw order
- From incident w/ lexicographically smallest destination

[x] incident loop
- ccw order
- From arbitrary incident

[x] each face
- lexicographic order from outer incident


## API

We are following the API design patterns of the standard library (containers and algorithms) as closely as my knowledge allows.

I mention baked DCEL's below, but I'm not sure that I'm going to implement it.

---
### Class Invariants
> These invariants are true after a method call iff it's true before the method call.

Otherwise behaviour is undefined.

#### Doubly Connected Edge Lists
- DCEL is valid.

#### Vertices
- Vertices are ordered lexicographically.
- No two verticies have the same coordinates.
- The incident of a vertex is the incident which has the lexicographically smallest destination.

#### Half Edges
- A half_edges source != it's destination.  Implied by "No two vertices have the same coordinates."

#### Faces
- The outer incident of a face is the incident which has the lexicographically smallest origin.
- The inner half_edges of a face are the half_edges with the lexicographically smallest origin in their half edge loop.
- The inner half_edges of a face are sorted lexicographically by origin.

----
### Definition
```cpp
template <Scalar, VertexData = void, HalfEdgeData = void, FaceData = void>
class half_edge_list
```

#### Methods

```cpp

/*

lexicographic vertex iterators.

*/
vertex_iterator begin_vertex() const noexcept;
vertex_iterator end_vertex() const noexcept;
vertex_iterator rbegin_vertex() const noexcept;
vertex_iterator rend_vertex() const noexcept;

/*

lexicographic face iterators.
lexicographic on outer.origin.

*/
face_iterator begin_face() const noexcept;
face_iterator end_face() const noexcept;
face_iterator rbegin_face() const noexcept;
face_iterator rend_face() const noexcept;

/*****************************************

add_edge()

Creates an edge with the two endpoints
  - Creates half edges required
  - Inserts half edges into the appropriate edge loops
  - Sets faces of half edges correctly, creating if required
    - If an edge bisects an existing face, the existing face belongs to the edge that is it's outer half edge and the new face is connected to the newly created half edge on.

Returns a pair where
  - first : An iterator pointing the the half edge corresponding to the first point of the edge.  If no edge was created, returns "end()".
  - second : True if an edge was created, otherwise false.

*/
  
pair<half_edge_iterator, bool> add_edge(const vec2<Scalar>& lhs, const vec2<Scalar>& rhs);
pair<half_edge_iterator, bool> add_edge(const segment2<Scalar>& seg);


/*****************************************

erase_edge()

Erases the specified edge, reconnecting edge loops, vertices and faces as needed.

Returns a pair where:
  - first : An iterator pointing to the face joined by the erasure.  If no edge was erased, "end()"
  - second : True if an edge was erased.  Otherwise false.

*/

pair<face_iterator, bool> erase_edge(const vec2<Scalar>&, const vec2<Scalar>&)
pair<faice_iterator, bool> erase_edge(const segment2<Scalar>&)
pair<face_iterator, bool> erase_edge(edge_iterator)


/*****************************************

erase_vertex()

Erases a vertex, merging all faces and connecting edge loops as necessary.
  - This has the potential to merge n-1 faces |-> 1 face.  Which face is selected???  Does it matter?

Returns a pair where:
  - first : An iterator pointing to the face that is a result of the merge.  otherwise "end()".
  - second : True if a vertex was removed.  Otherwise false.

Preconditions:
  - The length of all edge loops rooted at the vertex must be >= 3

*/

pair<face_iterator, bool> erase_vertex(const vec2<Scalar>&)
pair<face_iterator, bool> erase_vertex(vertex_iterator)

/*
get_vertex()

Gets the vertex at the position specified.

Returns an iterator pointing the the vertex at the position if it exsits.  Otherwise "end()".

*/
vertex_iterator get_vertex(const vec2<Scalar>&) const;
```

#### Nested Classes

```cpp
struct vertex {
  /*
    returns the incident with the lexocographically smallest destination.
  */
  half_edge_vertex_iterator begin();
  half_edge_vertex_iterator end();
  half_edge_vertex_iterator rbegin();
  half_edge_vertex_iterator rend();

  const vec2<Scalar> point() const;
  const incident& incident() const;
  ref<VertexData> data();
}

struct incident {
  /*
    iterators that access the edge loop from this incident
  */
  half_edge_loop_iterator begin();
  half_edge_loop_iterator end();
  half_edge_loop_iterator rbegin();
  half_edge_loop_iterator rend();

  const vertex& origin() const;
  const incident& twin() const;
  const incident& next() const;
  const incident& prev() const;
  const face& face() const;
  ref<HalfEdgeData> data();
}

struct face {
  const incident& outer() const;
  const set<ref<incident>>& inner() const;
}
```

### Post-Mortem

This didn't work.  There were some critical questions that I didnt ask when designing this API:

- Is it important that the construction of the half_edge_list is easy?
- Does the order of the member vertices/half_edges/faces matter?

##### Is it important that the construction of the half_edge_list is easy?

This is programming.  Tedium is part of the game.

The approach that I took was one where I was trying to make it completely brain-dead to construct a half edge list/dcel 
by letting the user of the api simply add edges arbitrarily.  This is not what will generally be happening in reality.  
I think.

What I do know is that trying to cover all of the edge cases that that approach forces on the developer difficult.  It also
requires the dev to make consessions on what does or does not enable the add_edge function to maintain an invariant that
the dcel is always correct.  These concessions would live in documentation and error messages which is less than ideal.  
It would be better for a single function to perform a single, specific operation with a simple invariant.  As is, add_edges
is trying to do too much overcomplicated.

##### Does the order of the member vertices/half_edges/faces matter?

In most cases, I don't think that it does.  Most operations involving half_edge_lists are going to be one and done or 
the user will know that a specific order is required and they can add it that way themselves.  So letting the developer
worry strictly about a simple, efficient api and leaving fanciness to the user will ensure a more robust code base.

#### Take-Aways

- Single Responsibility Principle.
  - Having a basic and functioning half_edge_list that simply manages the data is enough for a single class.
  - Fanciness can be added by inheritors/decorators.
- Ask _WHY_.
  - Do the managed items really need to be stored in sorted order?  Why?
  - Is it better for the memory of the data members to be contiguous?  Why?