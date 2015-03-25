#include <stdio.h>
#include <errno.h>
#include <malloc.h>
#include <string.h>

#include "mesh.h"
#include "off.h"
#include "draw.h"

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
  int *f_colors, *v_colors, *e_colors;
  int solid = 0;

  f_colors = (int*) calloc(mesh->num_faces, sizeof(int));
  v_colors = (int*) calloc(mesh->num_vertices, sizeof(int));
  e_colors = (int*) calloc(mesh->num_edges, sizeof(int));

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

    // Set the draw color of the target
    solid = 1;
    f_colors[target-1] = COLOR_TARGET;

    tmp = Face_adjacent(mesh, target-1, buf, 16);
    printf("Adjacent faces to triangle %zu:\n", target);
    for (i=0; i<tmp; i++) {
      printf("  %zu\n", buf[i] + 1);
      f_colors[buf[i]] = COLOR_ANSWER;
    }
    break;

  case EDGE_TRIANGLES:
    max_input = mesh->num_edges;
    printf("Enter the edge index [1-%zu]: ", max_input);
    ask_index(&target, max_input);

    solid = 1;
    e_colors[target-1] = COLOR_TARGET;
    e_colors[mesh->edges[target-1].pair] = COLOR_TARGET;

    HEdge_faces(mesh, target-1, buf, buf+1);
    printf("Edge %zu is shared by triangles %zu and %zu\n", target, buf[0]+1, buf[1]+1);
    memset(f_colors, COLOR_WIREONLY, mesh->num_faces);

    f_colors[buf[0]] = COLOR_ANSWER;
    f_colors[buf[1]] = COLOR_ANSWER;
    break;

  case VERTEX_FACES:
    max_input = mesh->num_vertices;
    printf("Enter the vertex index [1-%zu]: ", max_input);
    ask_index(&target, max_input);

    solid = 1;
    v_colors[target-1] = COLOR_TARGET;

    tmp = Vertex_faces(mesh, target-1, buf, 16);
    printf("Faces shared by vertex %zu:\n", target);
    memset(f_colors, COLOR_WIREONLY, mesh->num_faces);
    for (i=0; i<tmp; i++) {
      printf("  %zu\n", buf[i] + 1);
      f_colors[buf[i]] = COLOR_ANSWER;
    }
    break;

  case VERTEX_EDGES:
    max_input = mesh->num_vertices;
    printf("Enter the vertex index [1-%zu]: ", max_input);
    ask_index(&target, max_input);

    v_colors[target-1] = COLOR_TARGET;

    tmp = Vertex_edges(mesh, target-1, buf, 16);
    printf("Edges shared by vertex %zu:\n", target);
    for (i=0; i<tmp; i++) {
      printf("  %zu\n", buf[i] + 1);
      e_colors[buf[i]] = COLOR_ANSWER;
    }
    break;

  default:
    printf("Well, this should never happen. I hope you're happy.\n");
    break;
  }

  draw_model(mesh, f_colors, v_colors, e_colors, solid);

  free(f_colors);
  free(v_colors);
  free(e_colors);
}

int main(int argc, char **argv) {
  struct Mesh mesh;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s <off-file>\n", argv[0]);
    return 1;
  }

  read_off(&mesh, argv[1]);

  ask_question(&mesh);

  free(mesh.faces);
  free(mesh.vertices);
  free(mesh.edges);

  return 0;
}
