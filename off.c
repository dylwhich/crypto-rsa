#include "mesh.h"
#include "off.h"

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <malloc.h>

int read_off(struct Mesh *mesh, const char *filename) {
  char buf[512];
  FILE *file;
  size_t i, j;
  size_t tmp_n, tmp_v, edge_index;
  size_t first_edge, prev_edge;

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
  fscanf(file, "%zu %zu %zu\n", &(mesh->num_vertices), &(mesh->num_faces), &(mesh->num_edges));

  printf("%zu vertices, %zu faces, %zu edges\n", mesh->num_vertices, mesh->num_faces, mesh->num_edges);

  // Allocate arrays
  mesh->vertices = malloc(mesh->num_vertices * sizeof(struct Vertex));
  mesh->faces = malloc(mesh->num_faces * sizeof(struct Face));

  edge_index = 0;

  for (i = 0; i < mesh->num_vertices; i++) {
    fscanf(file, "%f %f %f\n", &(mesh->vertices[i].x),
	   &(mesh->vertices[i].y), &(mesh->vertices[i].z));
    printf("Read vertex %f, %f, %f\n", mesh->vertices[i].x, mesh->vertices[i].y,
	   mesh->vertices[i].z);
  }

  // for each face:
  for (i = 0; i < mesh->num_faces; i++) {
    fscanf(file, "%zu", &tmp_n);
    printf("Face %zu has %zu edges:\n", i, tmp_n);

    // Create the edge list, just a bit bigger than it needs to be
    if (i == 0) {
      mesh->edges = malloc(mesh->num_faces * 2 * tmp_n * sizeof(struct HEdge));
      mesh->num_edges = mesh->num_faces * 2 * tmp_n;
    }

    // for each vertex:
    for (j = 0; j < tmp_n; j++) {
      fscanf(file, " %zu", &tmp_v);
      printf("Vertex %zu: %zu\n", j, tmp_v);

      // Set the vertex and face for this edge
      mesh->edges[edge_index].vertex = tmp_v;
      mesh->edges[edge_index].face = i;

      // Conversely, set the face/vertex's edge to this
      mesh->faces[i].edge = edge_index;
      mesh->vertices[tmp_v].edge = edge_index;

      if (j == 0) {
	// Set the first edge so we can loop back to it
	first_edge = edge_index;
      } else {
	// Link the previous edge to this one
	mesh->edges[prev_edge].next = edge_index;
      }

      if (j == tmp_n - 1) {
	// We're on the last edge, link it to the first one
	mesh->edges[edge_index].next = first_edge;
      }

      // Store the previous index
      prev_edge = edge_index;

      // Move onto the next edge
      edge_index++;
    }

    fscanf(file, "\n");
  }

  // At this point, edge_index is equal to the total number of HEdges
  if (mesh->num_edges != edge_index) {
    // We now shrink it to fit, if necessary
    mesh->edges = realloc(mesh->edges, edge_index * sizeof(struct HEdge));
    mesh->num_edges = edge_index;
  }

  // Use brute-force to link the edges together
  // This runs in O(n^2)
  // But it's a little nicer n^2/2 actually...
  for (i = 0; i < mesh->num_edges; i++) {
    for (j = i+1; j < mesh->num_edges; j++) {
      if (mesh->edges[mesh->edges[i].next].vertex == mesh->edges[j].vertex &&
	  mesh->edges[mesh->edges[j].next].vertex == mesh->edges[i].vertex) {
	printf("Edges %zu and %zu are pairs!\n", i, j);
	mesh->edges[i].pair = j;
	mesh->edges[j].pair = i;
      }
    }
  }

  return 0;
}
