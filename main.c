#include <stdio.h>
#include <errno.h>

#include "mesh.h"
#include "off.h"

#define TRIANGLE_ADJACENT 1
#define EDGE_TRIANGLES 2
#define VERTEX_FACES 3
#define VERTEX_EDGES 4

void ask_index(size_t *result, size_t max) {
  while (!scanf("%zu", result) || *result < 1 || *result > max) {
    printf("Invalid input.\nTry again: ");
  }
}

void ask_question(struct Mesh *mesh) {
  int question;
  size_t target, max_input, tmp, i;
  size_t buf[16];

  printf("               - Questions -\n\n");
  printf(" 1. Given a triangle, what are the three adjacent triangles?\n");
  printf(" 2. Given an edge, which two triangles share it?\n");
  printf(" 3. Given a vertex, which faces share it?\n");
  printf(" 4. Given a vertex, which edges share it?\n\n");
  
  printf("Which question would you like to ask? [1-4]: ");
  while (!scanf("%d", &question) || question < 1 || question >4) {
    printf("Invalid input.\nTry again: ");
  }

  switch (question) {
  case TRIANGLE_ADJACENT:
    max_input = mesh->num_faces;
    printf("Enter the triangle index [1-%zu]: ", max_input);
    ask_index(&target, max_input);

    tmp = Face_adjacent(mesh, target-1, buf, 16);
    printf("Adjacent faces to triangle %zu:\n", target);
    for (i=0; i<tmp; i++) {
      printf("  %zu\n", buf[i] + 1);
    }
    break;

  case EDGE_TRIANGLES:
    max_input = mesh->num_edges;
    printf("Enter the edge index [1-%zu]: ", max_input);
    ask_index(&target, max_input);

    HEdge_faces(mesh, target-1, buf, buf+1);
    printf("Edge %zu is shared by triangles %zu and %zu\n", target, buf[0]+1, buf[1]+1);
    break;

  case VERTEX_FACES:
    max_input = mesh->num_vertices;
    printf("Enter the vertex index [1-%zu]: ", max_input);
    ask_index(&target, max_input);

    tmp = Vertex_faces(mesh, target-1, buf, 16);
    printf("Faces shared by vertex %zu:\n", target);
    for (i=0; i<tmp; i++) {
      printf("  %zu\n", buf[i] + 1);
    }

    break;

  case VERTEX_EDGES:
    max_input = mesh->num_vertices;
    printf("Enter the vertex index [1-%zu]: ", max_input);
    ask_index(&target, max_input);
    break;

  default:
    printf("Well, this should never happen. I hope you're happy.\n");
    break;
  }
}

int main(int argc, char **argv) {
  struct Mesh mesh;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s <off-file>\n", argv[0]);
    return 1;
  }

  read_off(&mesh, argv[1]);

  ask_question(&mesh);

  return 0;
}
