#ifndef DRAW_H
#define DRAW_H

#include "mesh.h"

// size of the OpenGL drawing window
#define WIN_HEIGHT 512
#define WIN_WIDTH  512

// color-names for drawing
#define COLOR_DEFAULT 0
#define COLOR_TARGET 1
#define COLOR_ANSWER 2

#define COLOR_VALUE_DEFAULT 1.0, 1.0, 1.0
#define COLOR_VALUE_TARGET 1.0, 0.0, 0.0
#define COLOR_VALUE_ANSWER 0.0, 1.0, 0.0

int draw_model(struct Mesh *mesh_in, int *face_colors_in,
	       int *vertex_colors_in, int *edge_colors_in, int solid_in);
void init();
void setColor(int colorVal);
void Display();
void Reshape(int width, int height);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);
void Keyboard(unsigned char key, int x, int y);
#endif
