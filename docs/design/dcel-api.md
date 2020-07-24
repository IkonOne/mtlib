# Half Edge List/Doubly Connected Edge List API Design

## Use Cases

### Algorithms

- Map overlay
  - using line segment intersection
    - Sorts line segments lexicographically
    - data is immutable
- Triangulation
  - Sorts vertices lexicographically
  - data is mutable where original data is immutable but additional edges can be added

- Construction
  - polygon soup
  - sub-region from face/edge_loop
  - vertex list/loop
  - edge list/loop
  - triangle strip
  - by hand

I'm becoming a fan of the builder pattern for fancy construction and let decl's be built by hand as well.

## Considerations

- Input
  - vertices
  - half_edges
  - faces
  - Stretch Goals
    - polygon soup
    - indexed data
- Queries
  - is_consistent(vertex/incident/face)
    - returns true if the item is combinatorially consistent.
- Modifiers
  - clear
  - reserve(vertex_count, half_edge_count, face_count)
    - allocates the appropriate amount of memory for each.
    - Probably only useful for vector/array backends???
  - vertex/incident/face
    - push_back
    - pop_back???
  - Stretch Goals
    - general combinatorial map modifiers
      - invariant: the dcel is consistent after execution iff the dcel is consistent prior.
      - These won't work well (or at all) with every type of internal ds.
        - Maybe reserve them for a builder pattern???
        - Or enable users to create specialized builders by providing different backend implementations
          - This would allow a user to construct the dcel with fast item lookup time or sorted items and then bake it to an array/vector backend for fast iteration.
          - e.g. map to ensure only a single vertex/incident exists at each location
          - set to ensure the same as map AND to sort the data in place.
      - add_inner_edge_to_face(v1, v2, face)
        - Adds an edge as an inner incident of the face.
        - preconditions: Edge cannot overlap an existing edge.  Neither vertex may exist.
      - add_inner_edge_list_to_face(vertex/edge list, outer_face, is_loop)
        - Adds an edge list as an inner component to a face.
        - preconditions : No edge of the list may overlap or be incident to an existing edge.
      - add_vertex(vertex, next_half_edge)
        - Inserts a vertex creating the edge before the next_half_edge on the edge_loop.
        - preconditions: new edge created as a result must not intersect an existing edge.
      - remove_vertex(vertex)
        - Removes a vertex
        - preconditions: Any edges removed as a result cannot leave an isolated vertex.
      - merge_faces_at(incident)
        - merges the faces of incident.face and incident.twin.face by removing the edge.
        - preconditions: incident.face != incident.twin.face
      - split_face_at(v1, v2)
        - splits the face into two.
        - preconditions: v1 and v2 must be on the outer edge loop of the face.
      - split_edge_at(incident, vertex)
        - splits an edge into two new edges whose destination is vertex.
        - does not require colinearity.
        - preconditions: the newly created edges must not overlap any existing edges.
- Data
  - Stretch Goals
    - Arbitrary data for items
  - vertex
    - position
    - incident
  - incident
    - origin
    - dest
    - twin
    - prev
    - next
    - face
  - face
    - outer_half_edge
    - Stretch Goals
        - inner_half_edges
- Iteration
  - all verticies
  - all half_edges
  - all faces
  - vertex_incident_half_edges
  - half_edge_loop
  - face_outer_half_edges
  - face_inner_half_edge_loops

### Classes

- dcel
  - Role: manages data of a dcel.
  - Responsibilities
    - add and remove vertices/half_edges/faces
    - provides access to data and iterators
    - allocate memory appropriately
    
- abstract_dcel_builder
  - Role: defines the interface for a dcel_builder
  - Responsibilities:
    - provides a simplified interface to build a dcel
    - performs common combinatorially correct operations on the dcel
    - Ensures data integrity
    - creates a dcel of the specified type
  - Concrete Implementations:
    - dcel_builder_map : The most basic builder that ensures only one of any object exists.
    - dcel_builder_set : Ensures only one of any object exists and keeps them sorted.
    
- dcel_vertex (implementation defined)
  - Role: provides an interface to a dcel_vertex_record
- dcel_half_edge (implementation defined)
  - Role: provides an interface to a dcel_half_edge_record
- dcel_face (implementation defined)
  - Role: provides an interface to a dcel_face_record

- dcel_vertex_record (implementation defined)
  - point
  - incident
- dcel_half_edge_record (implementation defined)
  - vertex
  - twin
  - prev
  - next
  - face
- dcel_face_record (implementation defined)
  - outer_half_edge
  - inner_half_edges

## Implementation plan

1. create std::list backed dcel with pointer based records.
2. create iterators
3. Add arbitrary data.
4. generalize to support std::vector backend.
5. create abstract_dcel_builder and dcel_builder_map
6. create dcel_builder_set

## Notes

### Building a dcel

The issue I am coming across right now is that of how to actually build a dcel given a set of geometry.

How do I create and store a vertex, incident and face such that the api is clean and easy to use for the end user?

As it stands with the std::list implementation, I am using pointers to the objects in the list
 itself.  That ensures that I have a single instance of any vertex.  It is up to the user of the
  DCEL to not add verticies twice.  Fine.

So do I simply give the iterator back to the user?  It will never be invalidated.
Do I give a handle back to the user?  This would allow for the possiblity of different back ends
 and for the records themselves to remain private/immutable directly by the user.

```
dcel_list d;
auto v1 = d.create_vertex();
auto v2 = d.create_vertex();
auto h1 = d.create_half_edge(v1);
auto h2 = d.create_half_edge(v2);
v1->incident = h1;
v2->incident = h2;
h1->twin = h1->prev = h1->next = h2;
h2->twin = h2->prev = h2->next = h1;
// half_edges are created with the boundary face by default.
assert(d.is_consistent(v1));
assert(d.is_consistent(v2));
assert(d.is_consistent(h1));
assert(d.is_consistent(h2));
```