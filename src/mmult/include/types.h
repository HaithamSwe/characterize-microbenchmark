#ifndef __INCLUDE_TYPES_H_
#define __INCLUDE_TYPES_H_

typedef struct {
  int* matrix_a;
  int* matrix_b;
  int* result;

  int M;          // Rows in A
  int N;          // Columns in A and rows in B
  int P;          // Columns in B

  int cpu;
  int nthreads;
} args_t;

#endif
