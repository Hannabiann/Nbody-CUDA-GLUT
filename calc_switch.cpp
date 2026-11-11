#include "global.h"
#include <GL/freeglut.h>
#include <cstdio>
#include <iostream>


extern void OMP_Math();
extern void GPU_Malloc();
extern void GPU_Math();
extern void GPU_Free();
extern void drawFrame();

//OpenMP = 1; CUDA_Force = 2; CUDA_Tile = 3;
int button = 3;
bool prepGPU = false;

void CallNextFrame(int)
{
	//CPU or GPU calc
	if (button != 1) {
		if (!prepGPU){
			GPU_Malloc();
			prepGPU = true;
		}
		GPU_Math();
	}
		
	else {
		if (prepGPU) {
			GPU_Free();
			prepGPU = false;
		}
		OMP_Math();
	}

	//Draw func and call build next frame
	drawFrame();
	glutTimerFunc(0, CallNextFrame, 0);
}

//Select CPU(1) or GPU(2,3) 
void keyboardPressed(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '1': button = 1;
		break;
	case '2': button = 2;
		break;
	case '3': button = 3;
		break;
	}
}