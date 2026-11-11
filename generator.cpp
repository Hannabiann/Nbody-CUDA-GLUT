#include "global.h"
#include <iostream>

const int N = 16000;
const int grav = 5000000;
const float EPS = 3E5;

//Point coord
float POSX[N], POSY[N], POSZ[N], POSR[N * 3];
//Point mass
float POSM[N];

//Point generator
void generateWorld()
{
	for (int i = 0; i < N; i++) {
		//Zone +LH
		if (i < (int)N / 8) {
			POSX[i] = rand() % 1000 - 2000;
			POSY[i] = rand() % 1000 + 1000;
			POSZ[i] = rand() % 1000 + 1000;			
		}
		//Zone +LL
		else if ((i >= (int)N / 8) && (i < (int)(N / 8) * 2)) {
			POSX[i] = rand() % 1000 - 2000;
			POSY[i] = rand() % 1000 + 1000;
			POSZ[i] = rand() % 1000 - 2000;
		}

		//Zone +RH
		else if ((i >= (int)(N / 8) * 2) && (i < (int)(N / 8) * 3)) {
			POSX[i] = rand() % 1000 + 1000;
			POSY[i] = rand() % 1000 + 1000;
			POSZ[i] = rand() % 1000 + 1000;
		}

		//Zone +RL
		else if ((i >= (int)(N / 8) * 3) && (i < (int)(N / 8) * 4)) {
			POSX[i] = rand() % 1000 + 1000;
			POSY[i] = rand() % 1000 + 1000;
			POSZ[i] = rand() % 1000 - 2000;
		}

		//Zone -LH
		else if ((i >= (int)(N / 8) * 4) && (i < (int)(N / 8) * 5)) {
			POSX[i] = rand() % 1000 - 2000;
			POSY[i] = rand() % 1000 - 2000;
			POSZ[i] = rand() % 1000 + 1000;
		}

		//Zone -LL
		else if ((i >= (int)(N / 8) * 5) && (i < (int)(N / 8) * 6)) {
			POSX[i] = rand() % 1000 - 2000;
			POSY[i] = rand() % 1000 - 2000;
			POSZ[i] = rand() % 1000 - 2000;
		}

		//Zone -RH
		else if ((i >= (int)(N / 8) * 6) && (i < (int)(N / 8) * 7)) {
			POSX[i] = rand() % 1000 + 1000;
			POSY[i] = rand() % 1000 - 2000;
			POSZ[i] = rand() % 1000 + 1000;
		}

		//Zone -RL
		else if ((i >= (int)(N / 8) * 7) && (i < N)) {
			POSX[i] = rand() % 1000 + 1000;
			POSY[i] = rand() % 1000 - 2000;
			POSZ[i] = rand() % 1000 - 2000;
		}

		for (int j = 0; j < i; j++) {
			if ((i != j) && (POSX[i] == POSX[j]) && (POSY[i] == POSY[j]) && (POSZ[i] == POSZ[j]))
				i--;
			else
				POSM[i] = rand() % 1000 + 1;
		}
	}
	POSM[N-1] = 100000;
	POSM[1] = 100000;
	POSM[2000] = 100000;
	POSM[4000] = 100000;
	POSM[6000] = 100000;
	POSM[8000] = 100000;
	POSM[10000] = 100000;
	POSM[12000] = 100000;
	POSM[14000] = 100000;
}