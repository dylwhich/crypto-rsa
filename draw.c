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

struct Spline *splines;
size_t num_splines;
GLfloat *plain_points;

void init(void)
{
  int total_points = 0;
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_FLAT);

  num_splines = 1;
  
  splines = malloc(sizeof(struct Spline) * num_splines);

  // TODO don't hardcode this!!!
  plain_points = malloc(3 * sizeof(GLfloat) * splines[0].num_points);

  plain_points[0] = -4.0;
  plain_points[1] = -4.0;
  plain_points[2] =  0.0;
  plain_points[3] = -2.0;
  plain_points[4] =  4.0;
  plain_points[5] =  0.0;
  plain_points[6] =  2.0;
  plain_points[7] = -4.0;
  plain_points[8] =  0.0;
  plain_points[9] =  4.0;
  plain_points[10] = 4.0;
  plain_points[11] = 0.0;

  glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4/*splines[0].num_points*/, plain_points);
  glEnable(GL_MAP1_VERTEX_3);
}

void display(void)
{
  int i;

  glClear(GL_COLOR_BUFFER_BIT);
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

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize (500, 500);
  glutInitWindowPosition (100, 100);
  glutCreateWindow (argv[0]);
  init ();
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutMainLoop();
  return 0;
}
