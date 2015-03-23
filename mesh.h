#ifndef MESH_H
#define MESH_H

struct HEdge;
struct Vertex;
struct Face;

struct HEdge {
  struct HEdge *pair, *next;
  struct Vertex *vertex;
  struct Face *face;
};

struct Vertex {
  struct HEdge *edge;
  float x, y, z;
};

struct Face {
  struct HEdge *edge;
};

#define Triangle Face

void Vertex_init(struct Vertex *this, struct HEdge *edge, float x, float y,
		 float z);
void HEdge_init(struct HEdge *this, struct HEdge *pair, struct HEdge *next,
		struct Vertex *vertex, struct Face *face);
void Face_adjacent(struct Face *this, struct Face *a, struct Face *b,
		   struct Face *c);
void HEdge_faces(struct HEdge *this, struct Face *a, struct Face *b);
void Vertex_faces(struct Vertex *this, struct Face **faces);
void Vertex_edges(struct Vertex, struct HEdge **edges);

#endif
