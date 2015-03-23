#include "mesh.h"
#include "off.h"

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <malloc.h>

int read_off(struct HEdge **edge_list, struct Vertex **vertex_list,
	      struct Face **face_list, const char *filename,
	      size_t *num_edges, size_t *num_vertices, size_t *num_faces) {
  char buf[512];
  FILE *file;
  size_t i, j;
  float tmp_x, tmp_y, tmp_z;
  size_t tmp_n, tmp_v, edge_index;
  struct HEdge *edge, *pair, *firstEdge, *firstPair, *prevEdge, *prevPair;

  // Open file
  errno = 0;
  file = fopen(filename, "r");

  if (file == NULL || errno != 0) {
    fprintf(stderr, "Could not open %s: %d\n", filename, errno);
    return 1;
  }

  // Check header
  fscanf(file, "%512s\n", buf);
  if (strcmp(buf, "OFF")) {
    fprintf(stderr, "%s is not an OFF file: expected 'OFF', got '%s'\n",
	    filename, buf);
    return 1;
  }

  // Read counts
  fscanf(file, "%zu %zu %zu\n", num_vertices, num_faces, num_edges);
  printf("%zu vertices, %zu faces, %zu edges\n", *num_vertices, *num_faces, *num_edges);

  // Allocate arrays
  *vertex_list = malloc(*num_vertices * sizeof(struct Vertex));
  *face_list = malloc(*num_faces * sizeof(struct Face));

  edge_index = 0;

  for (i = 0; i < *num_vertices; i++) {
    fscanf(file, "%f %f %f\n", &tmp_x, &tmp_y, &tmp_z);
    printf("Read vertex %f, %f, %f\n", tmp_x, tmp_y, tmp_z);
    Vertex_init(*vertex_list + i, NULL, tmp_x, tmp_y, tmp_z);
  }

  // for each face:
  for (i = 0; i < *num_faces; i++) {
    fscanf(file, "%zu", &tmp_n);
    printf("Face %zu has %zu edges:\n", i, tmp_n);

    if (i == 0) {
      *edge_list = malloc(*num_faces * 2 * tmp_n * sizeof(struct HEdge));
    }

    edge = NULL;
    pair = NULL;
    firstEdge = NULL;
    firstPair = NULL;
    prevEdge = NULL;
    prevPair = NULL;

    // for each vertex:
    for (j = 0; j < tmp_n; j++) {
      fscanf(file, " %zu", &tmp_v);
      printf("Vertex %zu: %zu\n", j, tmp_v);

      // get two uninitialized HEdge structs
      edge = *edge_list + edge_index++;
      pair = *edge_list + edge_index++;

      // set up the pair info
      edge->pair = pair;
      pair->pair = edge;
      edge->vertex = *vertex_list + tmp_v;

      if (firstEdge == NULL) {
	firstEdge = edge;
	firstPair = pair;
      } else {
	prevEdge->next = edge;
	//pair->next = prevPair;
	prevPair->vertex = *vertex_list + tmp_v;
      }

      if (j == (tmp_n - 1)) {
	edge->next = firstEdge;
	firstPair->next = pair;
      }

      prevEdge = edge;
      prevPair = pair;
    }
    fscanf(file, "\n");
  }

  return 0;
}
