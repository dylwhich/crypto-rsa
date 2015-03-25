#ifndef OFF_H
#define OFF_H

#include <stddef.h>

#define MODE_OFF 0
#define MODE_NOFF 1

int read_off(struct Mesh *mesh, const char *filename);

#endif
