#include "mesh.h"

void Face_adjacent(struct Face *this, struct Face *a, struct Face *b,
		   struct Face *c);
void HEdge_faces(struct HEdge *this, struct Face *a, struct Face *b);
void Vertex_faces(struct Vertex *this, struct Face **faces);
void Vertex_edges(struct Vertex, struct HEdge **edges);
