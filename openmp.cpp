#include "global.h"
#include <omp.h>
#include <cmath>

//CPU_OpenMP
void OMP_Math()
{
	float AX_M, AY_M, AZ_M, det_X, det_Y, det_Z, den, norm, ext;
#pragma omp parallel for private(den, AX_M, AY_M, AZ_M)
	for (int first = 0; first < N; first++) {
		AX_M = 0;
		AY_M = 0;
		AZ_M = 0;
		for (int next = 0; next < N; next++) {
			det_X = POSX[next] - POSX[first];
			det_Y = POSY[next] - POSY[first];
			det_Z = POSZ[next] - POSZ[first];
			norm = det_X*det_X + det_Y*det_Y + det_Z*det_Z;
			ext = norm * norm + EPS * EPS;
			den = grav * POSM[next]/ (sqrtf(ext) * ext);
			AX_M += det_X * den;
			AY_M += det_Y * den;
			AZ_M += det_Z * den;			
		}
		POSR[first] += AX_M;
		POSR[first + N] += AY_M;
		POSR[first + N + N] += AZ_M;

		POSX[first] += POSR[first];
		POSY[first] += POSR[first + N];
		POSZ[first] += POSR[first + N + N];
	}
}