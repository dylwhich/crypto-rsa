#include "mesh.h"

size_t Face_adjacent(struct Mesh *mesh, size_t face,
		     size_t *adjacent, size_t max_results) {
  size_t results = 0;
  size_t cur_edge = mesh->faces[face].edge;

  do {
    if (results == max_results) {
      return 0;
    }

    adjacent[results] = mesh->edges[mesh->edges[cur_edge].pair].face;
    cur_edge = mesh->edges[cur_edge].next;
    results++;
  }  while (cur_edge != mesh->faces[face].edge);

  return results;
}

void HEdge_faces(struct Mesh *mesh, size_t edge, size_t *face_a, size_t *face_b) {
  *face_a = mesh->edges[edge].face;
  *face_b = mesh->edges[mesh->edges[edge].pair].face;
}

size_t Vertex_faces(struct Mesh *mesh, size_t vertex,
		    size_t *faces, size_t max_results) {
  size_t results = 0;
  size_t cur_edge = mesh->vertices[vertex].edge;

  do {
    if (results == max_results) {
      return 0;
    }

    faces[results] = mesh->edges[cur_edge].face;
    cur_edge = mesh->edges[mesh->edges[cur_edge].pair].next;
    results++;
  }  while (cur_edge != mesh->vertices[vertex].edge);

  return results;
}

size_t Vertex_edges(struct Mesh *mesh, size_t vertex,
		    size_t *edges, size_t max_results) {
  return 0;
}
