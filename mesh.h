#ifndef MESH_H
#define MESH_H

#include <stddef.h>

struct HEdge;
struct Vertex;
struct Face;

struct Mesh {
  struct HEdge *edges;
  struct Vertex *vertices;
  struct Face *faces;
  size_t num_edges, num_vertices, num_faces;
};

struct HEdge {
  size_t pair, next;
  size_t vertex;
  size_t face;
};

struct Vertex {
  size_t edge;
  float x, y, z;
};

struct Face {
  size_t edge;
};

size_t Face_adjacent(struct Mesh *mesh, size_t face,
		     size_t *adjacent, size_t max_results);
void HEdge_faces(struct Mesh *mesh, size_t edge, size_t *face_a, size_t *face_b);
size_t Vertex_faces(struct Mesh *mesh, size_t vertex,
		    size_t *faces, size_t max_results);
size_t Vertex_edges(struct Mesh *mesh, size_t vertex,
		    size_t *edges, size_t max_results);

#endif
