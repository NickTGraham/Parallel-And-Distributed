/*
 * Original author:  Sandhya Dwarkadas, 2002 and before.
 * Modified by Grant Farmer, 2003 and Kai Shen, 2010.
 * Minor cleanup by Michael Scott, 2017.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <math.h>
#include <assert.h>
#include <mpi.h>

/* #define DEBUG */

#define SWAP(a, b)      {double tmp = a; a = b; b = tmp;}
#define SWAPPTR(a,b)    {void* tmp = a; a = b; b = tmp;}

struct pivotStruct {
    int rank;
    int pivotrow;
    double tmpval;
};

struct equationStruct {
    int rank;
    double *R;
    double **mat;
};

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
  for (i = 0; i < nsize; i++) {
    X__[i] = i+1;
  }

  R = (double*) malloc(nsize * sizeof(double));
  assert(R != NULL);
  for (i = 0; i < nsize; i++) {
    R[i] = 0.0;
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
  for (i = 0; i < nsize; i++) {
    X[i] = 0.0;
  }
}

// Get the pivot - make sure the next row is the one whose initial value
// has the largest absolute value.
//
void getPivot(int nsize, int currow, int* pivotrow, double* tmpval, int numtasks, int rank) {
  int i;
  //Set up values for dividing the work
  int stepsize = (nsize-currow)/numtasks;
  int threadstart = rank*stepsize;
  int threadend = (rank+1)*stepsize;
  stepsize = (rank == 0)?stepsize+1:stepsize;
  if (rank == numtasks-1 && (threadend) != nsize) {
      threadend = nsize;
  }
  *tmpval = matrix[currow][currow];
  *pivotrow = currow;
  for (i = threadstart + currow; i < threadend + currow; i++) {
    if (fabs(matrix[i][currow]) > fabs(matrix[*pivotrow][currow])) {
      *pivotrow = i;
      *tmpval = matrix[i][currow];
    }
  }

  if (fabs(matrix[*pivotrow][currow]) == 0.0) {
    fprintf(stderr, "The matrix is singular\n");
    exit(-1);
  }

//   if (pivotrow != currow) {
// #ifdef DEBUG
//     fprintf(stdout, "pivot row at step %5d is %5d\n", currow, pivotrow);
// #endif
//     SWAPPTR(matrix[pivotrow], matrix[currow]);
//     SWAP(R[pivotrow], R[currow]);
//   }
}

// For all the rows, get the pivot and eliminate all rows and columns
// for that particular pivot row.
//
void computeGauss(int nsize, int numtasks, int rank) {
  int i, j, k, stepsize, threadstart, threadend, pivotrow;
  double pivotval, tmpval;

  MPI_Status status;
  for (i = 0; i < nsize; i++) {
    getPivot(nsize, i, &pivotrow, &tmpval, numtasks, rank);
    //Put the values found into our struct

    if (rank == 0) {
        int child_row;
        double child_val;
        //Receive the values from all other workers
        for (int j = 1; j < numtasks; j++) {
            MPI_Recv(&child_row, 1, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
            MPI_Recv(&child_val, 1, MPI_DOUBLE, status.MPI_SOURCE, 1, MPI_COMM_WORLD, &status);
            if (child_val > tmpval) {
                tmpval = child_val;
                pivotrow = child_row;
            }
        }
        //printf("Root Pivot: [%d] = %f\n", pivotrow, matrix[pivotrow][i]);
        //Send the "consensus" result to all the children
        MPI_Bcast(&pivotrow, 1, MPI_INT, 0, MPI_COMM_WORLD);
    }
    else {
        //Send that struct to Rank 0 with tag 1
        MPI_Send(&pivotrow, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
        MPI_Send(&tmpval, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
        //Receive the computed results from all the threads from Rank 0
        //MPI_Recv(&pivotresults, 1, mpipivotstruct, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        MPI_Bcast(&pivotrow, 1, MPI_INT, 0, MPI_COMM_WORLD);
        //printf("Child Pivot: [%d] = %f\n", pivotrow, matrix[pivotrow][i]);
    }

    //Now that we have "reached consensus" each worker must swap the vals & ptrs
    if (pivotrow != i) {
    #ifdef DEBUG
        fprintf(stdout, "pivot row at step %5d is %5d\n", currow, pivotrow);
    #endif
        SWAPPTR(matrix[pivotrow], matrix[i]);
        SWAP(R[pivotrow], R[i]);
    }

    //Set up values for dividing the work
    stepsize = (nsize-i)/numtasks;
    threadstart = rank*stepsize + i;
    threadend = (rank+1)*stepsize + i;
    threadstart = (rank == 0)?threadstart+1:threadstart;

    if (rank == numtasks-1 && (threadend) != nsize) {
        threadend = nsize;
    }
    //printf("%d, %d, %d, %d\n", i, rank, threadstart, threadend);
    // Scale the main row:
    pivotval = matrix[i][i];

    if (pivotval != 1.0) {
      matrix[i][i] = 1.0;
      for (j = threadstart; j < threadend; j++) {
        matrix[i][j] /= pivotval;
      }
      R[i] /= pivotval;
    }
    // Factorize the rest of the matrix:
    //for (j = threadstart + i; j < threadend + i; j++) {
    for (int j = i+1; j < nsize; j++) {
      pivotval = matrix[j][i];
      matrix[j][i] = 0.0;
      //for (k = i + 1; k < nsize; k++) {
      for (k = threadstart; k <threadend; k++) {
        matrix[j][k] -= pivotval * matrix[i][k];
      }
      R[j] -= pivotval * R[i];
    }

    //problem, this requires a sync every itteration which is costly...
    //Seems to be the only way to do it I think, without destroying the purpose
    //looks like all will cast and the band of rows/cols that the worker used
    //will be merged by the master thread, then cast outward for all to see
    //Need to merge both R and matrix
    if (rank == 0) {
        double* child_R = (double*) malloc(nsize * sizeof(double));
        double** child_mat = (double**) malloc(nsize * sizeof(double*));
        double *tmp = (double*) malloc(nsize * nsize * sizeof(double));
        assert(tmp != NULL);
        for (j = 0; j < nsize; j++) {
          child_mat[j] = tmp;
          tmp += nsize;
        }
        //Receive the values from all other workers
        for (int j = 1; j < numtasks; j++) {

            MPI_Recv(child_R, nsize, MPI_DOUBLE, MPI_ANY_SOURCE, 8, MPI_COMM_WORLD, &status);
            MPI_Recv(child_mat[0], nsize*nsize, MPI_DOUBLE, status.MPI_SOURCE, 9, MPI_COMM_WORLD, &status);
            int childstart = status.MPI_SOURCE * stepsize + i;
            int childend = (status.MPI_SOURCE + 1) * stepsize + i;
            if (status.MPI_SOURCE == numtasks-1 && (childend) != nsize) {
                childend = nsize;
            }
            for (int k = childstart; k < childend; k++) {
                R[k] = child_R[k];
                for (int m = i; m < nsize; m++) {
                    matrix[m][k] = child_mat[m][k];
                }
            }
        }

        MPI_Bcast(R, nsize, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Bcast(matrix[0], nsize*nsize, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        free(child_mat[0]);
        free(child_mat);
        free(child_R);
    }
    else {
        MPI_Send(R, nsize, MPI_DOUBLE, 0, 8, MPI_COMM_WORLD);
        MPI_Send(matrix[0], nsize*nsize, MPI_DOUBLE, 0, 9, MPI_COMM_WORLD);
        MPI_Bcast(R, nsize, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Bcast(matrix[0], nsize*nsize, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }
  }
}

// Solve the equation.
//
void solveGauss(int nsize) {
  int i, j;

  X[nsize-1] = R[nsize-1];
  for (i = nsize - 2; i >= 0; i --) {
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
  int i, numtasks, rank;
  struct timeval start, finish;
  int nsize = 0;
  double error;


  if (argc != 2) {
    fprintf(stderr, "usage: %s <matrixfile>\n", argv[0]);
    exit(-1);
  }

  nsize = initMatrix(argv[1]);
  initRHS(nsize);
  initResult(nsize);

  gettimeofday(&start, 0);
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  printf("Hello from %d\n", rank);
  printf("total %d\n", numtasks);
  computeGauss(nsize, numtasks, rank);
  MPI_Finalize();
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
