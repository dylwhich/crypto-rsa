#include "bezier.h"

#include <stdio.h>

void spline_init(struct Spline *this) {
  this->point = NULL;
  this->num_points = 0;
}

void spline_destroy(struct Spline *this) {
  struct ControlPoint *cur = this->point;
  struct ControlPoint *tmp;

  while (cur != NULL) {
    tmp = cur->next;
    control_point_destroy(cur);
    free(cur);

    cur = tmp;
  }

  this->point = NULL;
}

struct ControlPoint *spline_add_control_point(struct Spline *this, struct ControlPoint *before) {
  struct ControlPoint *cur = this->point;
  struct ControlPoint *new_point = (struct ControlPoint*) malloc(sizeof(struct ControlPoint));

  control_point_init(new_point, this);

  // Seek so that 'cur' points to the 
  while (cur != NULL && cur->next != before) cur = cur->next;

  new_point->next = before;

  if (cur != NULL) {
    cur->next = new_point;
  } else {
    this->point = new_point;
  }

  this->num_points++;

  return new_point;
}

struct ControlPoint *spline_add_control_point_at(struct Spline *this, struct ControlPoint *before, GLfloat x, GLfloat y, GLfloat z) {
  struct ControlPoint *cur = this->point;
  struct ControlPoint *new_point = (struct ControlPoint*) malloc(sizeof(struct ControlPoint));

  control_point_init_coord(new_point, this, x, y, z);

  // Seek so that 'cur' points to the 
  while (cur != NULL && cur->next != before) cur = cur->next;

  new_point->next = before;

  if (cur != NULL) {
    cur->next = new_point;
  } else {
    this->point = new_point;
  }

  this->num_points++;

  return new_point;
}

void spline_remove_control_point(struct Spline *this, struct ControlPoint *point) {
  struct ControlPoint *cur = this->point;

  while (cur != NULL && cur->next != point) cur = cur->next;

  this->num_points--;
  cur->next = point->next;
  control_point_destroy(point);
  free(point);
}

void spline_save_array(struct Spline *this, GLfloat *target) {
  struct ControlPoint *cur = this->point;
  int i = 0;

  while (cur != NULL) {
    target[i++] = cur->x;
    target[i++] = cur->y;
    target[i++] = cur->z;

    cur = cur->next;
  }
}


void control_point_init(struct ControlPoint *this, struct Spline *parent) {
  this->x = 0.0;
  this->y = 0.0;
  this->z = 0.0;

  this->spline = parent;
  this->next = NULL;
}

void control_point_init_coord(struct ControlPoint *this, struct Spline *parent, GLfloat x, GLfloat y, GLfloat z) {
  this->x = x;
  this->y = y;
  this->z = z;

  this->spline = parent;
  this->next = NULL;
}

void control_point_init_coord_next(struct ControlPoint *this, struct Spline *parent, GLfloat x, GLfloat y, GLfloat z, struct ControlPoint *next) {
  control_point_init_coord(this, parent, x, y, z);
  this->next = next;
}

void control_point_destroy(struct ControlPoint *this) {

}
