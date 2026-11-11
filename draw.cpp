#include "global.h"
#include <GL/freeglut.h>
#include <cstdio>
#include <iostream>
//For rotate
static float rot_x = 0, rot_y = 0, x_angle = 0, y_angle = 0;

//Mouse rotation
void MousePressedMove(int ay, int ax)
{
	glutSetCursor(GLUT_CURSOR_NONE);
	int x_center = (int)h / 2, y_center = (int)w / 2;
	if (ax != x_center || ay != y_center) {
		if (x_center < ax)
			x_angle += 6;
		if (x_center > ax)
			x_angle -= 6;
		if (y_center < ay)
			y_angle += 6;
		if (y_center > ay)
			y_angle -= 6;
		glutWarpPointer(x_center, y_center);
	}
}

void initializationGlut()
{
	glClearColor(0, 0, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glOrtho(-4000, 4000, -4000, 4000, -4000, 4000);
}

void drawFrame()
{
	//FPS meter
	static float fps, time_f, timebase = 0;
	static int frames = 0;
	static char res[50];
	const char *name_method;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glPushMatrix();
	glRotatef(x_angle, 1.0, 0.0, 0.0); //Rot axis X
	glRotatef(y_angle, 0.0, 1.0, 0.0); //Rot axis Y

	glBegin(GL_POINTS);
	for (int draw = 0; draw < N; draw++) {
		glColor3f(1, 1, 1);
		glVertex3f(POSX[draw], POSY[draw], POSZ[draw]);
	}
	glEnd();

	glBegin(GL_LINES);
	//Axis X
	glColor3f(1, 0, 0);
	glVertex3f(-2000.0, -2000.0, -2000.0);
	glVertex3f(-1000.0, -2000.0, -2000.0);
	//Axis Y
	glColor3f(0, 1, 0);
	glVertex3f(-2000.0, -2000.0, -2000.0);
	glVertex3f(-2000.0, -1000.0, -2000.0);
	//Axis Z
	glColor3f(0, 0, 1);
	glVertex3f(-2000.0, -2000.0, -2000.0);
	glVertex3f(-2000.0, -2000.0, -1000.0);
	glEnd();

	glPopMatrix();
	glDisable(GL_DEPTH_TEST);
	glutSwapBuffers();

	//Program name and FPS on window title
	frames++;
	time_f = glutGet(GLUT_ELAPSED_TIME);
	if (time_f - timebase >= 1000)
	{
		fps = frames*1000.0 / (time_f - timebase);
		timebase = time_f;
		frames = 0;
		
		if (button == 2)
			name_method = "CUDA Force";
		else if (button == 3)
			name_method = "CUDA Tile";
		else
			name_method = "OpenMP";

		sprintf(res, "Nbody V3.2.1 | Method: %s | FPS: %.3f", name_method, fps); //version
		glutSetWindowTitle(res);
	}
}