#include <stdio.h>

#include "mesh.h"
#include "off.h"

int main(int argc, char **argv) {
  struct HEdge *edge_list;
  struct Vertex *vertex_list;
  struct Face *face_list;

  size_t num_edges, num_vertices, num_faces;


  if (argc != 2) {
    fprintf(stderr, "Usage: %s <off-file>\n", argv[0]);
    return 1;
  }

  read_off(&edge_list, &vertex_list, &face_list, argv[1], &num_edges, &num_vertices, &num_faces);

  return 0;
}
