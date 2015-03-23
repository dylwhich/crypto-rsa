#ifndef OFF_H
#define OFF_H

#include <stddef.h>

int read_off(struct HEdge **edge_list, struct Vertex **vertex_list,
	      struct Face **face_list, const char *filename,
	      size_t *num_edges, size_t *num_vertices, size_t *num_faces);
int read_off_sizes(const char *filename, int *num_edges, int *num_vertices,
		    int *num_faces);

#endif
