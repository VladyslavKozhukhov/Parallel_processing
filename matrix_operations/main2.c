#include <stdio.h>
#include "mpi.h"
#define N 3


void printMatrixOnScreen(int **matrix)
{
	int i, j;
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
			printf("%d ", matrix[i][j]);
		printf("\n");
	}
	return;
}
void printArrayOnScreen(int * arr)
{
	int i;
	for (i = 0; i < N*N; i++)
		printf("%d ", arr[i]);
	printf("\n");
}


int vector_mult_vector(int *vect1, int *vect2)
{
	/*
		-- this preforms vector X vector, sizes (1xN) X (Nx1)
	*/
	int i;
	int result = 0;
	for (i = 0; i < N; i++)
	{
		result = result + vect1[i] * vect2[i];
	}
	return result;
}

int* create_col_vect(int ** matrix, int index)
{
	/*
		-- this creates a vector of a column from a matrix
		-- for example we got the matrix 1 2 3 
										 4 5 6
										 7 8 9
		-- create_col_vect(matrix, 1) will return 2 5 8
	*/
	int i;
	int *res = (int *)malloc(N * sizeof(int));
	for (i = 0; i < N; i++)
	{
		res[i] = matrix[i][index];
	}
	return res;
}

int* vect_mult_mat(int* vect, int ** matrix)
{
	/*
		-- this preforms "vector shura" x matrix, will return "vector shura"
		-- thats the function the workers will use.
	*/
	int i;
	int *res = (int *)malloc(N * sizeof(int));
	for (i = 0; i < N; i++)
	{
		res[i] = vector_mult_vector(vect, create_col_vect(matrix,i));
	}
	return res;
}

int* createArrayFromMatrix(int ** matrix)
{
	int i,j;
	int counter = 0;
	int *A = (int *)malloc(N*N * sizeof(int));
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			A[counter] = matrix[i][j];
			counter++;
		}
	}
	return A;
}

int ** createMatrixFromArray(int * arr)
{
	int i, j;
	int counter = 0;
	int **MAT = (int **)malloc(N * sizeof(int *));
	for (i = 0; i < N; i++)
		MAT[i] = (int *)malloc(N * sizeof(int));

	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			MAT[i][j] = arr[counter];
			counter++;
		}
	}
	return MAT;
}


int main()
{
	int i, j;
	
	int numprocs, myRank;
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

	int start_location = 2, dest = 1, tag = 0;

	if (myRank == 0)
		//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ rank 0
	{
		int counter = 0;
		// create and init a matrix, where MATRIX[i][j] means in ROW i and COL j
		int **matrixA = (int **)malloc(N * sizeof(int *));
		for (i = 0; i < N; i++)
			matrixA[i] = (int *)malloc(N * sizeof(int));
		for (i = 0; i < N; i++)
		{
			for (j = 0; j < N; j++)
			{
				matrixA[i][j] = counter;
				counter++;
			}
		}
		int **matrixB = (int **)malloc(N * sizeof(int *));
		for (i = 0; i < N; i++)
			matrixB[i] = (int *)malloc(N * sizeof(int));
		for (i = 0; i < N; i++)
		{
			for (j = 0; j < N; j++)
			{
				matrixB[i][j] = counter;
				counter++;
			}
		}
		printMatrixOnScreen(matrixA);
		printf("==\n");
		printMatrixOnScreen(matrixB);
		printf("==\n");

		
		/*
		int *tmp = (int *)malloc(N*N * sizeof(int));
		tmp = createArrayFromMatrix(matrixA);
		MPI_Send(tmp, N*N, MPI_INT, dest, tag, MPI_COMM_WORLD);
		*/
		

	}
	else
	{
		//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ SLAVE
		int numrows, rec_data2, rec_data3;

		/*
		int *tmp = (int *)malloc(N*N * sizeof(int));
		
		int **rec = (int **)malloc(N * sizeof(int *));
		for (i = 0; i < N; i++)
			rec[i] = (int *)malloc(N * sizeof(int));
		
		
		MPI_Recv(tmp, N*N, MPI_INT, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		rec = createMatrixFromArray(tmp);
		printf("process number %d recieved %d start location\n", myRank, rec_data1);
		printf("process number %d recieved matrix:\n", myRank);
		printMatrixOnScreen(rec);
		*/
		

	}


	MPI_Finalize();
	
	
	
	return 0;
}

/*
	// test the vector_mult_vector
	int vect1[4] = { 1, 2, 3, 4};
	int vect2[4] = { 2, 3, 4, 5 };
	int result = vector_mult_vector(vect1, vect2);
	*/
	/*
	// test the create_col_vect
	int *result = (int *)malloc(N * sizeof(int));
	result = create_col_vect(matrixA, 2);
	for (i = 0; i < N; i++)
		printf("%d ", result[i]);
	*/
	/*
	test of vect_mult_mat
	int vect1[4] = { 1, 2, 3, 4 };
	int *result = (int *)malloc(N * sizeof(int));
	result = vect_mult_mat(vect1, matrixA);
	for (i = 0; i < N; i++)
		printf("%d ", result[i]);
	*/