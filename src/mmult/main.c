#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "common/types.h"
#include "impl/naive.h"
#include "impl/opt.h"
#include "include/types.h"

int main(int argc, char **argv) {
    int M = 512, N = 512, P = 512, nruns = 1, block_size = 64;

    int opt;
    while ((opt = getopt(argc, argv, "M:N:P:R:B:")) != -1) {
        switch (opt) {
        case 'M':
            M = atoi(optarg);
            if (M <= 0) exit(EXIT_FAILURE);
            break;
        case 'N':
            N = atoi(optarg);
            if (N <= 0) exit(EXIT_FAILURE);
            break;
        case 'P':
            P = atoi(optarg);
            if (P <= 0) exit(EXIT_FAILURE);
            break;
        case 'R':
            nruns = atoi(optarg);
            if (nruns <= 0) exit(EXIT_FAILURE);
            break;
        case 'B':
            block_size = atoi(optarg);
            if (block_size <= 0) exit(EXIT_FAILURE);
            break;
        default:
            fprintf(stderr,
                    "Usage: %s -M <rows_in_A> -N <cols_in_A_and_rows_in_B> -P <cols_in_B> [-R <num_runs>] [-B <block_size>]\n",
                    argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    printf("Parsed dimensions: M = %d, N = %d, P = %d, nruns = %d, B = %d\n", M, N, P, nruns, block_size);

    int *A = malloc(M * N * sizeof(int));
    int *B = malloc(N * P * sizeof(int));
    int *R_naive = malloc(M * P * sizeof(int));
    int *R_opt = malloc(M * P * sizeof(int));
    if (!A || !B || !R_naive || !R_opt) exit(EXIT_FAILURE);

    srand(time(NULL));
    for (int i = 0; i < M * N; i++) A[i] = rand() % 10;
    for (int i = 0; i < N * P; i++) B[i] = rand() % 10;

    args_t args_naive = {.matrix_a = A, .matrix_b = B, .result = R_naive, .M = M, .N = N, .P = P};
    args_t args_opt = {.matrix_a = A, .matrix_b = B, .result = R_opt, .M = M, .N = N, .P = P};

    // Naive Implementation Timing
    printf("\nRunning naive implementation:\n");
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int run = 0; run < nruns; run++) {
        impl_scalar_naive(&args_naive);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    double naive_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("Naive execution time for %d runs: %.6f seconds.\n", nruns, naive_time);

    // Optimized Implementation Timing
    printf("\nRunning optimized implementation:\n");
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int run = 0; run < nruns; run++) {
        impl_scalar_opt(&args_opt, block_size);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    double opt_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("Optimized execution time for %d runs: %.6f seconds.\n", nruns, opt_time);

    // Compare Results
    int mismatch = 0;
    for (int i = 0; i < M * P; i++) {
        if (R_naive[i] != R_opt[i]) {
            mismatch = 1;
            fprintf(stderr, "ERROR: Results mismatch at index %d: naive = %d, opt = %d\n", i, R_naive[i], R_opt[i]);
            break;
        }
    }
    if (!mismatch) {
        printf("Results match between naive and optimized implementations.\n");
    }

    // Performance Speedup
    printf("\nPerformance Speedup: %.2fx\n", naive_time / opt_time);

    free(A);
    free(B);
    free(R_naive);
    free(R_opt);

    return 0;
}

