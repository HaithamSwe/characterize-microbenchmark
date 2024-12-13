#include "common/types.h"
#include "include/types.h"
#include <stdlib.h>
#include <stddef.h>

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

void* impl_scalar_opt(void* args, int block_size) {
    args_t* arguments = (args_t*)args;

    int* A = (int*)arguments->matrix_a;
    int* B = (int*)arguments->matrix_b;
    int* R = malloc(arguments->M * arguments->P * sizeof(int));

    int M = arguments->M;
    int N = arguments->N;
    int P = arguments->P;

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < P; j++) {
            R[i * P + j] = 0;
        }
    }

    for (int ii = 0; ii < M; ii += block_size) {
        for (int jj = 0; jj < P; jj += block_size) {
            for (int kk = 0; kk < N; kk += block_size) {
                for (int i = ii; i < MIN(ii + block_size, M); i++) {
                    for (int j = jj; j < MIN(jj + block_size, P); j++) {
                        int sum = 0;
                        for (int k = kk; k < MIN(kk + block_size, N); k++) {
                            sum += A[i * N + k] * B[k * P + j];
                        }
                        R[i * P + j] += sum;
                    }
                }
            }
        }
    }

    arguments->result = R;
    return NULL;
}
