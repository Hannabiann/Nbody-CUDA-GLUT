#include "global.h"
#include <GL/freeglut.h>
#include <iostream>
#include <ctime>

extern void cudaInitDevices();
extern void generateWorld();
extern void initializationGlut();
extern void CallNextFrame(int);
extern void drawFrame();
extern void keyboardPressed(unsigned char key, int x, int y);
extern void MousePressedMove(int ay, int ax);


const int h = 1000;
const int w = 1000;

int main(int argc, char **argv)
{
	srand(time(0));
	setlocale(LC_ALL, "russian");

	//Call generator
	generateWorld();

	cudaInitDevices();

	std::cout << "\n\nN: " << N << std::endl << std::endl;
	std::cout << "[1] -- Select CPU (OpenMP)\n";
	std::cout << "[2] -- Select CUDA Force Calc\n";
	std::cout << "[3] -- Select CUDA Tile Calc\n";
	std::cout << "[Move + click] -- Rotate" << std::endl << std::endl;

	//For freeglut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(h, w);
	glutCreateWindow("");
	glutDisplayFunc(drawFrame);
	glutTimerFunc(0, CallNextFrame, 0);
	glutKeyboardFunc(keyboardPressed);
	glutMotionFunc(MousePressedMove);
	initializationGlut();
	glutMainLoop();

	return 0;
}