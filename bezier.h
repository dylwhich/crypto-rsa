#ifndef BEZIER_H
#define BEZIER_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

struct Spline {
  struct ControlPoint *point;
  size_t num_points;
};

struct ControlPoint {
  GLfloat x, y, z;
  struct ControlPoint *next;
  struct Spline *spline;
};

void spline_init(struct Spline *this);
void spline_destroy(struct Spline *this);
struct ControlPoint *spline_add_control_point(struct Spline *this, struct ControlPoint *before);
struct ControlPoint *spline_add_control_point_at(struct Spline *this, struct ControlPoint *before, GLfloat x, GLfloat y, GLfloat z);
void spline_remove_control_point(struct Spline *this, struct ControlPoint *point);
void spline_save_array(struct Spline *this, GLfloat* target);

void control_point_init(struct ControlPoint *this, struct Spline *parent);
void control_point_init_coord(struct ControlPoint *this, struct Spline *parent, GLfloat x, GLfloat y, GLfloat z);
void control_point_init_coord_next(struct ControlPoint *this, struct Spline *parent, GLfloat x, GLfloat y, GLfloat z, struct ControlPoint *next);
void control_point_destroy(struct ControlPoint *this);

#endif
