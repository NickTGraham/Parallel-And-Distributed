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

#define PRINTER 0
/* #define DEBUG */

#define SWAP(a, b)      {double tmp = a; a = b; b = tmp;}
#define SWAPPTR(a,b)    {void* tmp = a; a = b; b = tmp;}

MPI_Comm subgroup;

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
    int stepsize = (nsize)/numtasks;
    int threadstart = rank*stepsize;
    int threadend = (rank+1)*stepsize;
    stepsize = (rank == 0)?stepsize+1:stepsize;
    if (rank == numtasks-1 && (threadend) != nsize) {
        threadend = nsize;
    }

    /* Its not your job my friend, give up */
    if (!(threadstart <= currow && currow < threadend)) {
        *pivotrow = -1;
        *tmpval = 0;
        return;
    }

    *tmpval = matrix[currow][currow];
    *pivotrow = currow;
    for (i = currow + 1; i < nsize; i++) {
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
    double* pivotvals = (double*) malloc(nsize * sizeof(double));
    //Set up values for dividing the work
    stepsize = (nsize)/numtasks;
    threadstart = rank*stepsize;
    threadend = (rank+1)*stepsize;
    threadstart = (rank == 0)?threadstart+1:threadstart;
    if (rank == numtasks-1 && (threadend) != nsize) {
        threadend = nsize;
    }
    MPI_Status status;
    for (i = 0; i < nsize; i++) {
        getPivot(nsize, i, &pivotrow, &tmpval, numtasks, rank);
        /* So the i/stepsize worker has the value, make them broadcast it out */
        MPI_Bcast(&pivotrow, 1, MPI_INT, i/stepsize, subgroup);

        //Now that we have "reached consensus" each worker must swap the vals & ptrs
        if (pivotrow != i) {
            #ifdef DEBUG
            fprintf(stdout, "pivot row at step %5d is %5d\n", currow, pivotrow);
            #endif
            SWAPPTR(matrix[pivotrow], matrix[i]);
            SWAP(R[pivotrow], R[i]);
        }

        // Scale the main row:
        pivotval = matrix[i][i];
        /* So the i/stepsize worker has the value, make them broadcast it out */
        MPI_Bcast(&pivotval, 1, MPI_DOUBLE, i/stepsize, subgroup);
        if (pivotval != 1.0) {
            matrix[i][i] = 1.0;
            for (j = threadstart; j < threadend; j++) {
                matrix[i][j] /= pivotval;
            }
            R[i] /= pivotval;
        }
        // Factorize the rest of the matrix:
        assert(pivotvals != NULL);
        if (threadstart-1 <= i && i < threadend) {
            for (int y = 0; y < i+1; y++) {
                pivotvals[y] = 0;
            }
            for (int y = i+1; y < nsize; y++) {
                pivotvals[y] = matrix[y][i];
            }
        }

        MPI_Bcast(pivotvals, nsize, MPI_DOUBLE, i/stepsize, subgroup);
        for (int j = i+1; j < nsize; j++) {
            //pivotval = matrix[j][i];
            pivotval = pivotvals[j];
            matrix[j][i] = 0.0;
            for (k = threadstart; k < threadend; k++) {
                matrix[j][k] -= pivotval * matrix[i][k];
            }
            R[j] -= pivotval * R[i];
        }
        /*
        if (rank == 0) {
            printf("%d M = [", rank);
            for (int m = 0; m < nsize; m++) {
                for (int n = 0; n < nsize; n++) {
                    printf("%f, ", matrix[m][n]);
                }
                printf("\n");
            }
            printf("]\n");
        }
        */
        if (threadstart-1 <= i && i < threadend) {
            threadstart++;
        }
    }


    if (rank == 0) {
        double** child_mat = (double**) malloc(nsize * sizeof(double*));
        double *tmp2 = (double*) malloc(nsize * nsize * sizeof(double));
        assert(tmp2 != NULL);

        for (j = 0; j < nsize; j++) {
            child_mat[j] = tmp2;
            tmp2 += nsize;
        }

        //Receive the values from all other workers
        for (int j = 1; j < numtasks; j++) {
            MPI_Recv(child_mat[0], nsize*nsize, MPI_DOUBLE, MPI_ANY_SOURCE, 9, subgroup, &status);
            int childstart = status.MPI_SOURCE * stepsize;
            int childend = (status.MPI_SOURCE + 1) * stepsize;
            if (status.MPI_SOURCE == numtasks-1 && (childend) != nsize) {
                childend = nsize;
            }
            for (int k = childstart; k < childend; k++) {
                //R[k] = child_R[k];
                for (int m = 0; m < nsize; m++) {
                    matrix[m][k] = child_mat[m][k];
                }
            }
        }
        free(child_mat[0]);
        free(child_mat);
    }
    else {
        MPI_Send(matrix[0], nsize*nsize, MPI_DOUBLE, 0, 9, subgroup);
    }
    free(pivotvals);
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

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    /* these are tmp cheap fixes */
    if (numtasks > nsize/2) {
        numtasks = nsize/2;
    }
    else if (nsize%numtasks != 0) {
        numtasks -= nsize%numtasks;
    }
    printf("Hello from %d\n", rank);
    printf("total %d\n", numtasks);

    gettimeofday(&start, 0);
    if (rank >= numtasks) {
        printf("exiting [%d] when [%d]\n", rank, numtasks);
        MPI_Comm_split(MPI_COMM_WORLD, 10, rank, &subgroup);
        MPI_Finalize();
        exit(0);
    }
    MPI_Comm_split(MPI_COMM_WORLD, 1, rank, &subgroup);
    //MPI_Barrier(MPI_COMM_WORLD);
    computeGauss(nsize, numtasks, rank);
    MPI_Finalize();
    if (rank != 0) {
        exit(0);
    }
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

    exit(0);
}
