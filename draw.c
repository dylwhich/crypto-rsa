/* 
* Adapted from http://www.glprogramming.com/red/chapter12#name2
 */
//#include "trackball.h"
#include "bezier.h"
//#include "draw.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>

#define BBOX_WIDTH 0.1
#define BBOX_HEIGHT 0.1

struct Spline *splines;
size_t num_splines;
GLfloat *plain_points;

struct ControlPoint *selected_point;
int dragging = 0;
int drag_x, drag_y;

void init(void)
{
  struct Spline *cur_spline;
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_FLAT);

  num_splines = 1;
  
  splines = malloc(sizeof(struct Spline) * num_splines);

  cur_spline = splines;
  spline_init(cur_spline);
  spline_add_control_point_at(cur_spline, NULL, -4.0, -4.0, 0.0);
  spline_add_control_point_at(cur_spline, NULL, -2.0, 4.0, 0.0);
  spline_add_control_point_at(cur_spline, NULL, 2.0, -4.0, 0.0);
  spline_add_control_point_at(cur_spline, NULL, 4.0, 4.0, 0.0);

  // TODO don't hardcode this!!!
  plain_points = malloc(3 * sizeof(GLfloat) * splines[0].num_points);

  spline_save_array(cur_spline, plain_points);

  glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4/*splines[0].num_points*/, plain_points);
  glEnable(GL_MAP1_VERTEX_3);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
}

void display(void)
{
  int i;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glColor3f(1.0, 1.0, 1.0);
  glBegin(GL_LINE_STRIP);
  for (i = 0; i <= 30; i++)
    glEvalCoord1f((GLfloat) i/30.0);
  glEnd();
  /* The following code displays the control points as dots. */
  glPointSize(5.0);
  glColor3f(1.0, 1.0, 0.0);
  glBegin(GL_POINTS);
  for (i = 0; i < 4; i++)
    glVertex3fv(plain_points + i * 3);
    glEnd();
  glFlush();
}

void reshape(int w, int h)
{
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (w <= h)
    glOrtho(-5.0, 5.0, -5.0*(GLfloat)h/(GLfloat)w,
	    5.0*(GLfloat)h/(GLfloat)w, -5.0, 5.0);
  else
    glOrtho(-5.0*(GLfloat)w/(GLfloat)h,
	    5.0*(GLfloat)w/(GLfloat)h, -5.0, 5.0, -5.0, 5.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

struct ControlPoint *check_collide(int x, int y) {
  struct Spline *cur_spline;
  struct ControlPoint *cur_point;

  GLint view[4];
  GLfloat winx, winy, winz;

  glGetIntegerv(GL_VIEWPORT, view);

  winx = 10.0 * ((float)x / view[2]) - 5.0;
  winy = -(10.0 * ((float)y / view[3]) - 5.0);

  printf("Click at %f, %f\n", winx, winy);

  for (cur_spline = splines; cur_spline != splines + num_splines; cur_spline++) {
    cur_point = cur_spline->point;

    while (cur_point != NULL) {
      if (cur_point->x >= winx - BBOX_WIDTH && cur_point->x <= winx + BBOX_WIDTH &&
	  cur_point->y >= winy - BBOX_HEIGHT && cur_point->y <= winy + BBOX_HEIGHT) {
	printf("Collide!\n");
	return cur_point;
      }

      cur_point = cur_point->next;
    }
  }
  return NULL;
}

void mouse(int button, int state, int x, int y) {
  const char *button_l = "LEFT", *button_r = "RIGHT", *button_m = "MIDDLE";
  const char *up = "UP", *down="DOWN";

  const char *s_button, *s_state;

  switch (button) {
  case 0:
    s_button = button_l;
    break;

  case 1:
    s_button = button_r;
    break;

  case 2:
    s_button = button_m;
    break;
  }

  switch (state) {
  case 0:
    s_state = down;
    check_collide(x, y);
    break;

  case 1:
    s_state = up;
    if (dragging) {
      printf("Dragging DONE -- (%d, %d)\n", drag_x, drag_y);
    } else {
      printf("Clicked -- (%d, %d)\n", x, y);
    }
    dragging = 0;
    break;
  }

  printf("%s %4s (%d, %d)\n", s_button, s_state, x, y);
}

void motion(int x, int y) {
  dragging = 1;
  drag_x = x;
  drag_y = y;
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize (500, 500);
  glutInitWindowPosition (100, 100);
  glutCreateWindow (argv[0]);
  init ();
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutMainLoop();
  return 0;
}
