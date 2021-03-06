/*
 * Original author:  Sandhya Dwarkadas, 2002 and before.
 * Modified by Grant Farmer, 2003 and Kai Shen, 2010.
 * Minor cleanup by Michael Scott, 2017.
 *
 * Modified by Nick Graham, 2017 adding in openMP Parallelization
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <math.h>
#include <assert.h>

#include <omp.h>

/* #define DEBUG */

#define SWAP(a, b)      {double tmp = a; a = b; b = tmp;}
#define SWAPPTR(a,b)    {void* tmp = a; a = b; b = tmp;}

/*
 * The code in this file takes as input a matrix of the following format:
 *    line 1: rows cols rows*cols
 *      (we require that rows == cols)
 *    last line: 0 0 0
 *    intermediate lines: i j v
 *      indicating that matrix[i-1][j-1] == v
 *
 * We then choose a right-hand side R such that the vector [1 2 3 ... rows]
 * will be a solution to the equation
 *      matrix * X = R
 *
 * Finally, we solve the equation and verify that we got the expected
 * solution.
 */

double **matrix, *X, *R;

double *X__;        // pre-set solution

// Initialize the matrix.
//
int initMatrix(const char *fname) {
  FILE *file;
  int l1, l2, l3;
  double d;
  int nsize;
  int i, j;
  double *tmp;
  char buffer[1024];

  if ((file = fopen(fname, "r")) == NULL) {
    fprintf(stderr, "Matrix file open error\n");
    exit(-1);
  }

  // Parse the first line to get the matrix size:
  fgets(buffer, 1024, file);
  assert(sscanf(buffer, "%d %d %d", &l1, &l2, &l3) == 3);
  nsize = l1;
  assert(l2 == l1 && l3 == l1 * l2);
#ifdef DEBUG
  fprintf(stdout, "matrix size is %d\n", nsize);
#endif

  // Initialize the space and set all elements to zero:
  matrix = (double**) malloc(nsize * sizeof(double*));
  assert(matrix != NULL);
  tmp = (double*) malloc(nsize * nsize * sizeof(double));
  assert(tmp != NULL);
  for (i = 0; i < nsize; i++) {
    matrix[i] = tmp;
    tmp += nsize;
  }
  for (i = 0; i < nsize; i++) {
    for (j = 0; j < nsize; j++) {
      matrix[i][j] = 0.0;
    }
  }

  // Parse the rest of the input file to fill the matrix:
  for (;;) {
    fgets(buffer, 1024, file);
    assert(sscanf(buffer, "%d %d %lf", &l1, &l2, &d) == 3);
    if (l1 == 0) {
      assert(l2 == 0 && d == 0);
      break;
    }
    assert(0 < l1 && l1 <= nsize);
    assert(0 < l2 && l1 <= nsize);

    matrix[l1-1][l2-1] = d;
#ifdef DEBUG
    fprintf(stdout, "row %d column %d of matrix is %e\n",
        l1-1, l2-1, matrix[l1-1][l2-1]);
#endif
  }

  fclose(file);
  return nsize;
}

// Initialize the right-hand-side following the pre-set solution.
//
void initRHS(int nsize) {
  int i, j;

  X__ = (double*) malloc(nsize * sizeof(double));
  assert(X__ != NULL);
  #pragma omp parallel for private(i) shared(X__)
  for (i = 0; i < nsize; i++) {
    X__[i] = i+1;
  }

  R = (double*) malloc(nsize * sizeof(double));
  assert(R != NULL);
  #pragma omp parallel for private(i) shared(R)
  for (i = 0; i < nsize; i++) {
    R[i] = 0.0;
    #pragma omp parallel for private(j) shared(R)
    for (j = 0; j < nsize; j++) {
      R[i] += matrix[i][j] * X__[j];
    }
  }
}

// Initialize the results.
//
void initResult(int nsize) {
  int i;

  X = (double*) malloc(nsize * sizeof(double));
  assert(X != NULL);
  #pragma omp parallel for private(i) shared(X)
  for (i = 0; i < nsize; i++) {
    X[i] = 0.0;
  }
}

// Get the pivot - make sure the next row is the one whose initial value
// has the largest absolute value.
//
void getPivot(int nsize, int currow) {
  int i, pivotrow;

  pivotrow = currow;
  #pragma omp parallel for private(i) shared(pivotrow)
  for (i = currow+1; i < nsize; i++) {
    if (fabs(matrix[i][currow]) > fabs(matrix[pivotrow][currow])) {
      pivotrow = i;
    }
  }

  if (fabs(matrix[pivotrow][currow]) == 0.0) {
    fprintf(stderr, "The matrix is singular\n");
    exit(-1);
  }

  if (pivotrow != currow) {
#ifdef DEBUG
    fprintf(stdout, "pivot row at step %5d is %5d\n", currow, pivotrow);
#endif
    SWAPPTR(matrix[pivotrow], matrix[currow]);
    SWAP(R[pivotrow], R[currow]);
  }
}

// For all the rows, get the pivot and eliminate all rows and columns
// for that particular pivot row.
//
void computeGauss(int nsize) {
  int i, j, k;
  double pivotval;
  printf("Max: %d\n", omp_get_max_threads());
  //#pragma omp parallel for private(i, pivotval) shared(matrix, R)
  for (i = 0; i < nsize; i++) {

    getPivot(nsize, i);

    // Scale the main row:
    pivotval = matrix[i][i];
    if (pivotval != 1.0) {
      matrix[i][i] = 1.0;
      #pragma omp parallel for shared(matrix)
      for (j = i + 1; j < nsize; j++) {
        printf("%d\n", omp_get_thread_num());
        matrix[i][j] /= pivotval;
      }
      R[i] /= pivotval;
    }

    // Factorize the rest of the matrix:
    #pragma omp parallel for shared(matrix, R) private(j, pivotval)
    for (j = i + 1; j < nsize; j++) {
      pivotval = matrix[j][i];
      matrix[j][i] = 0.0;
      #pragma omp parallel for shared(matrix) private(k)
      for (k = i + 1; k < nsize; k++) {
        matrix[j][k] -= pivotval * matrix[i][k];
      }
      R[j] -= pivotval * R[i];
    }
  }
}

// Solve the equation.
//
void solveGauss(int nsize) {
  int i, j;

  X[nsize-1] = R[nsize-1];
  for (i = nsize - 2; i >= 0; i --) {
    //printf("%d\n", omp_get_thread_num());
    X[i] = R[i];
    for (j = nsize - 1; j > i; j--) {
      X[i] -= matrix[i][j] * X[j];
    }
  }

#ifdef DEBUG
  fprintf(stdout, "X = [");
  for (i = 0; i < nsize; i++) {
    fprintf(stdout, "%.6f ", X[i]);
  }
  fprintf(stdout, "];\n");
#endif
}

int main(int argc, char *argv[]) {
  int i;
  struct timeval start, finish;
  int nsize = 0;
  double error;

  if (argc != 4) {
    fprintf(stderr, "usage: %s <matrixfile>\n", argv[0]);
    exit(-1);
  }

  nsize = initMatrix(argv[1]);
  initRHS(nsize);
  initResult(nsize);

  /* Set number of threads as a command line parameter */
  omp_set_dynamic(0);
  omp_set_num_threads(atoi(argv[2]));
  /* Select the Memory Order */
  if (strcmp(argv[3], "d") == 0) {
      /* Request Chunks when finished when needed */
      omp_set_schedule(omp_sched_dynamic, -1);
  }
  else if (strcmp(argv[3], "s") == 0) {
      /* Assign set chunks to each thread */
      omp_set_schedule(omp_sched_static, 1);
  }
  else if (strcmp(argv[3], "b") == 0) {
      /* Assign set chunks of given block size to each thread */
      omp_set_schedule(omp_sched_static, nsize/atoi(argv[2]));
  }
  else {
      /* Like dynamic, but size of chunks changes over time */
      omp_set_schedule(omp_sched_guided, -1);
  }

  nsize = initMatrix(argv[1]);
  initRHS(nsize);
  initResult(nsize);

  gettimeofday(&start, 0);
  computeGauss(nsize);
  gettimeofday(&finish, 0);

  solveGauss(nsize);

  fprintf(stdout, "Time:  %f seconds\n", (finish.tv_sec - start.tv_sec)
      + (finish.tv_usec - start.tv_usec) * 0.000001);

  error = 0.0;
  for (i = 0; i < nsize; i++) {
    double error__ = (X__[i]==0.0) ? 1.0 : fabs((X[i]-X__[i]) / X__[i]);
    if (error < error__) {
      error = error__;
    }
  }
  fprintf(stdout, "Error: %e\n", error);

  return 0;
}
