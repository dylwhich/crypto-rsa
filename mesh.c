#include "mesh.h"

void Vertex_init(struct Vertex *this, struct HEdge *edge, float x, float y,
		 float z) {
  this->edge = edge;
  this->x = x;
  this->y = y;
  this->z = z;
}
void HEdge_init(struct HEdge *this, struct HEdge *pair, struct HEdge *next,
		struct Vertex *vertex, struct Face *face) {
  this->pair = pair;
  this->next = next;
  this->vertex = vertex;
  this->face = face;
}

void Face_adjacent(struct Face *this, struct Face *a, struct Face *b,
		   struct Face *c);
void HEdge_faces(struct HEdge *this, struct Face *a, struct Face *b);
void Vertex_faces(struct Vertex *this, struct Face **faces);
void Vertex_edges(struct Vertex, struct HEdge **edges);
