#include <stdlib.h>
#include "common/macros.h"
#include "common/types.h"
#include "include/types.h"

void* impl_scalar_naive(void* args) {
    args_t* arguments = (args_t*)args;

    int* A = (int*)arguments->matrix_a;
    int* B = (int*)arguments->matrix_b;
    int* R = malloc(arguments->M * arguments->P * sizeof(int));

    for (int i = 0; i < arguments->M; i++) {
        for (int j = 0; j < arguments->P; j++) {
            R[i * arguments->P + j] = 0;
            for (int k = 0; k < arguments->N; k++) {
                R[i * arguments->P + j] += A[i * arguments->N + k] * B[k * arguments->P + j];
            }
        }
    }

    arguments->result = R;
    return NULL;
}
