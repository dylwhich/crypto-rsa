/*
  @@ v1: Jian Chen (jichen@umbc.edu)
  @@     CMSC 435 / 634 project 1:  main function
  @@     display my cool models
  @@     August 2013
*/

#ifdef __APPLE__
  #include <GLUT/glut.h>
#else
  #include <GL/glut.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#include "draw.h"
#include "trackball.h"
#include "mesh.h"

// object rotation along the three axes
extern GLfloat zoom; 
extern GLfloat tbx;
extern GLfloat tby;
GLint  mouseButton = -1;
struct Mesh *mesh;
int *face_colors, *vertex_colors, *edge_colors;
int solid;

void init() {
  /* background color: black */
  glClearColor(0, 0, 0, 0);
  glShadeModel(GL_FLAT);

  tbInit(GLUT_LEFT_BUTTON);
}

void setColor(int colorVal) {
  switch(colorVal) {
  case COLOR_TARGET:
    glColor3f(COLOR_VALUE_TARGET);
    break;
  case COLOR_ANSWER:
    glColor3f(COLOR_VALUE_ANSWER);
    break;
  case COLOR_DEFAULT:
  default:
    glColor3f(COLOR_VALUE_DEFAULT);
    break;
  }
}

void Display() {
  int face, vertex, edge;

  glClear(GL_COLOR_BUFFER_BIT);

  glPushMatrix();
  tbMatrix();

  glDisable(GL_LIGHTING);
  glDisable(GL_TEXTURE_2D);

  if (mesh != NULL) {
    for (face = 0; face < mesh->num_faces; face++) {

      // Draw the edges of the face
      edge = mesh->faces[face].edge;

      glBegin(GL_LINE_LOOP);
      do {
	vertex = mesh->edges[edge].vertex;
	setColor(edge_colors[edge]);

	glVertex3f(mesh->vertices[vertex].x,
		   mesh->vertices[vertex].y,
		   mesh->vertices[vertex].z);

	edge = mesh->edges[edge].next;
      } while (edge != mesh->faces[face].edge);
      glEnd();

      // If we should, draw the solid faces
      if (solid) {
	edge = mesh->faces[face].edge;

	glBegin(GL_TRIANGLE_STRIP);
	do {
	vertex = mesh->edges[edge].vertex;
	setColor(edge_colors[edge]);

	glVertex3f(mesh->vertices[vertex].x,
		   mesh->vertices[vertex].y,
		   mesh->vertices[vertex].z);
	} while (edge != mesh->faces[face].edge);
	glEnd();
      }
    }
  }

  glutSwapBuffers();
}

/*---------------------------------------------------------------
 *   Reshape function 
 *     defines how to redraw the view when the window is resized
 */
void Reshape(int width, int height) {
  tbReshape(width, height);

  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, (GLdouble)width/height, 0.01, 600);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0, 0, -10 + zoom);
}

void Mouse(int button, int state, int x, int y) {
   mouseButton=button;

   tbMouse(button, state, x, y);
   glutPostRedisplay();
}

void Motion(int x, int y) {
  tbMotion(x, y, mouseButton);
  glutPostRedisplay();
}

void Keyboard(unsigned char key, int x, int y) {
  switch(key) {
  case 27:   // esc
    exit(0);
    break;
  default:
    break;
  }
  glutPostRedisplay();
}

int draw_model(struct Mesh *mesh_in, int *face_colors_in,
	       int *vertex_colors_in, int *edge_colors_in, int solid_in) {
  int fake_argc = 0;
  char *fake_argv = "no";

  mesh = mesh_in;
  face_colors = face_colors_in;
  vertex_colors = vertex_colors_in;
  edge_colors = edge_colors_in;
  solid = solid_in;
  
  glutInit(&fake_argc, &fake_argv);
  glutInitWindowPosition(0, 0);
  glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutCreateWindow("Model\n");

  init();

  glutDisplayFunc(Display);
  glutReshapeFunc(Reshape);
  glutMouseFunc(Mouse);
  glutMotionFunc(Motion);
  glutKeyboardFunc(Keyboard);

  glutMainLoop();
  return 0;
}
