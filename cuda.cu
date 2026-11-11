#include "global.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <cmath>
#include <cstdio>

//For CUDA
const int blocksize = 500; //1024 max
int nblocks = N / blocksize;
float *CU_POSM, *CU_POSR, *CU_POSX, *CU_POSY, *CU_POSZ;


//CUDA_Force_Calc
__global__ void KernelSlow(float * __restrict__ POSM, float * __restrict__ POSX, float * __restrict__ POSY, float * __restrict__ POSZ, float * __restrict__ POSR, const int N, const int grav, const float EPS)
{
	float AX_M, AY_M, AZ_M, det_X, det_Y, det_Z, den, norm, ext;
	int first = threadIdx.x + blockIdx.x * blockDim.x;
	AX_M = 0;
	AY_M = 0;
	AZ_M = 0;
	for (int next = 0; next < N; next++) {
			det_X = POSX[next] - POSX[first];
			det_Y = POSY[next] - POSY[first];
			det_Z = POSZ[next] - POSZ[first];
			norm = det_X*det_X + det_Y*det_Y + det_Z*det_Z;
			ext = norm * norm + EPS * EPS;
			den = grav * POSM[next] * 1 / (sqrtf(ext) * ext);
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


//CUDA_Tile_Calc
__global__ void KernelFast(float * __restrict__ POSM, float * __restrict__ POSX, float * __restrict__ POSY, float * __restrict__ POSZ, float * __restrict__ POSR, const int N, const int grav, const float EPS)
{
	__shared__ float LPOSX[blocksize], LPOSY[blocksize], LPOSZ[blocksize], LPOSM[blocksize];

	float AX_M = 0, AY_M = 0, AZ_M = 0, den, norm, ext;
	int ia = blockDim.x * blockIdx.x + threadIdx.x;
	float POSX0 = POSX[ia], POSY0 = POSY[ia], POSZ0 = POSZ[ia], POSM0 = POSM[ia];

	for (unsigned int ib = 0; ib < N; ib += blocksize) {

		LPOSX[threadIdx.x] = POSX[ib + threadIdx.x];
		LPOSY[threadIdx.x] = POSY[ib + threadIdx.x];
		LPOSZ[threadIdx.x] = POSZ[ib + threadIdx.x];
		LPOSM[threadIdx.x] = POSM[ib + threadIdx.x];
		__syncthreads();

#pragma unroll
		for (unsigned int ic = 0; ic < blocksize; ic++) {			
			float det_X = (LPOSX[ic] - POSX0);
			float det_Y = (LPOSY[ic] - POSY0);
			float det_Z = (LPOSZ[ic] - POSZ0);
			norm = det_X*det_X + det_Y*det_Y + det_Z*det_Z;
			ext = norm * norm + EPS * EPS;			
			den = grav * LPOSM[ic] / (sqrtf(ext) * ext);
			AX_M += det_X * den;
			AY_M += det_Y * den;
			AZ_M += det_Z * den;			
		}
		__syncthreads();
	}
	POSR[ia] += AX_M;
	POSR[ia + N] += AY_M;
	POSR[ia + N + N] += AZ_M;

	POSX[ia] += POSR[ia];
	POSY[ia] += POSR[ia + N];
	POSZ[ia] += POSR[ia + N + N];
}


void GPU_Malloc()
{
	cudaMalloc((void**)&CU_POSM, N * sizeof(float));
	cudaMalloc((void**)&CU_POSX, N * sizeof(float));
	cudaMalloc((void**)&CU_POSY, N * sizeof(float));
	cudaMalloc((void**)&CU_POSZ, N * sizeof(float));
	cudaMalloc((void**)&CU_POSR, 3 * N * sizeof(float));

	cudaMemcpy(CU_POSM, POSM, N * sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(CU_POSX, POSX, N * sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(CU_POSY, POSY, N * sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(CU_POSZ, POSZ, N * sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(CU_POSR, POSR, 3 * N * sizeof(float), cudaMemcpyHostToDevice);
}


//CUDA Host CODE
void GPU_Math()
{
	if (button == 3)
		KernelFast <<<nblocks, blocksize>>> (CU_POSM, CU_POSX, CU_POSY, CU_POSZ, CU_POSR, N, grav, EPS);
		
	else if (button == 2)
		KernelSlow <<<nblocks, blocksize>>> (CU_POSM, CU_POSX, CU_POSY, CU_POSZ, CU_POSR, N, grav, EPS);

		cudaMemcpy(POSX, CU_POSX, N * sizeof(float), cudaMemcpyDeviceToHost);
		cudaMemcpy(POSY, CU_POSY, N * sizeof(float), cudaMemcpyDeviceToHost);
		cudaMemcpy(POSZ, CU_POSZ, N * sizeof(float), cudaMemcpyDeviceToHost);
		cudaMemcpy(POSR, CU_POSR, 3 * N * sizeof(float), cudaMemcpyDeviceToHost);
}

void GPU_Free()
{
	cudaFree(CU_POSM);
	cudaFree(CU_POSX);
	cudaFree(CU_POSY);
	cudaFree(CU_POSZ);
	cudaFree(CU_POSR);
}