#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "common/types.h"
#include "impl/naive.h"
#include "include/types.h"

int main(int argc, char **argv) {
  int M = 512;
  int N = 512;
  int P = 512;
  int nruns = 1;

  int opt;
  while ((opt = getopt(argc, argv, "M:N:P:R:")) != -1) {
    switch (opt) {
    case 'M':
      M = atoi(optarg);
      if (M <= 0) {
        fprintf(stderr,
                "ERROR: Matrix dimension M must be a positive integer.\n");
        exit(EXIT_FAILURE);
      }
      break;
    case 'N':
      N = atoi(optarg);
      if (N <= 0) {
        fprintf(stderr,
                "ERROR: Matrix dimension N must be a positive integer.\n");
        exit(EXIT_FAILURE);
      }
      break;
    case 'P':
      P = atoi(optarg);
      if (P <= 0) {
        fprintf(stderr,
                "ERROR: Matrix dimension P must be a positive integer.\n");
        exit(EXIT_FAILURE);
      }
      break;
    case 'R': // For number of runs
      nruns = atoi(optarg);
      if (nruns <= 0) {
        fprintf(stderr,
                "ERROR: Number of runs must be a positive integer.\n");
        exit(EXIT_FAILURE);
      }
      break;
    default:
      fprintf(stderr,
              "Usage: %s -M <rows_in_A> -N <cols_in_A_and_rows_in_B> -P "
              "<cols_in_B> [-R <num_runs>]\n",
              argv[0]);
      exit(EXIT_FAILURE);
    }
  }

  printf("Parsed dimensions: M = %d, N = %d, P = %d, nruns = %d\n", M, N, P,
         nruns);

  int *A = malloc(M * N * sizeof(int));
  int *B = malloc(N * P * sizeof(int));
  int *R = malloc(M * P * sizeof(int));

  if (A == NULL || B == NULL || R == NULL) {
    fprintf(stderr, "ERROR: Unable to allocate memory for matrices. Check your "
                    "system's memory capacity.\n");
    free(A);
    free(B);
    free(R);
    exit(EXIT_FAILURE);
  }

  srand(time(NULL));
  for (int i = 0; i < M * N; i++) {
    A[i] = rand() % 10;
  }
  for (int i = 0; i < N * P; i++) {
    B[i] = rand() % 10;
  }

  args_t args = {.matrix_a = A,
                 .matrix_b = B,
                 .result = R,
                 .M = M,
                 .N = N,
                 .P = P,
                 .cpu = 0,
                 .nthreads = 1};

  printf("Running matrix-matrix multiplication (%d x %d) x (%d x %d):\n", M, N,
         N, P);

  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC, &start);

  for (int run = 0; run < nruns; run++) {
    impl_scalar_naive(&args);
  }

  clock_gettime(CLOCK_MONOTONIC, &end);

  double elapsed = (end.tv_sec - start.tv_sec) +
                   (end.tv_nsec - start.tv_nsec) / 1e9;

  printf("Execution completed.\n");
  printf("Time taken for %d runs: %.6f seconds.\n", nruns, elapsed);

  if (M <= 10 && P <= 10) {
    printf("Result matrix (R):\n");
    for (int i = 0; i < M; i++) {
      for (int j = 0; j < P; j++) {
        printf("%d ", R[i * P + j]);
      }
      printf("\n");
    }
  }

  free(A);
  free(B);
  free(R);

  return 0;
}
