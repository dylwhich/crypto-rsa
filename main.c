#include <stdio.h>

#include "mesh.h"
#include "off.h"

int main(int argc, char **argv) {
  struct Mesh mesh;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s <off-file>\n", argv[0]);
    return 1;
  }

  read_off(&mesh, argv[1]);

  return 0;
}
