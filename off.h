#ifndef OFF_H
#define OFF_H

#include <stddef.h>

int read_off(struct Mesh *mesh, const char *filename);
int read_off_sizes(const char *filename, int *num_edges, int *num_vertices,
		    int *num_faces);

#endif
