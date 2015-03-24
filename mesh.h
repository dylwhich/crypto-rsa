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

#define Triangle Face

void Face_adjacent(struct Face *this, struct Face *a, struct Face *b,
		   struct Face *c);
void HEdge_faces(struct HEdge *this, struct Face *a, struct Face *b);
void Vertex_faces(struct Vertex *this, struct Face **faces);
void Vertex_edges(struct Vertex, struct HEdge **edges);

#endif
