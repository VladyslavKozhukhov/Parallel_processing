#include <stdio.h>
#include "mpi/mpi.h"
//#include "mpi.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "math.h"
#include <time.h>
int N = 5;

#define fromSlave 2
#define fromMaster 1

//void fillMatrix(double **matrix){
//    for(int  i = 0; i<  N  ; i++) {
//        for (int j = 0; j < N; j++) {
//            matrix[i][j] = i + 1.0;
//        }
//    }
//
//}
void fillMatrix(double *matrix){
    for(int  i = 0; i<  N  ; i++) {
        for (int j = 0; j < N; j++) {
            matrix[j+ i*N]= i + 1.0;
        }
    }

}
//void printMatrix(double **matrix){
//
//    for(int  i = 0; i<  N  ; i++) {
//        for (int j = 0; j < N; j++) {
//            printf("%lf ", matrix[i][j]);
//        }
//        printf("\n");
//    }
//}
void printMatrix(double *matrix){

    for(int  i = 0; i<  N  ; i++) {
        for (int j = 0; j < N; j++) {
            printf("%lf ", matrix[j+ i*N]);
        }
        printf("\n");
    }
}
double serialComputing(int**matrix){
//    int** matrixNew = (int**)(malloc(sizeof(int*)* (hight+1)));
//    for(int i=0; i < hight; i++){
//        matrixNew[i] = (int*)(malloc(sizeof(int)*(width+1)));
//    }
//    double Time_on,Time_off;
//    Time_on = MPI_Wtime(); //timer on
//    for(int i =0;i < hight;++i){
//        for(int j = 0; j < width;++j ) {
//            if (i != 0 && i != hight - 1 && j != 0 && j != width - 1) {
//                double y = 1.0 / 9.0;
//                double x = 1.0 * matrix[i - 1][j] * matrix[i - 1][j - 1] * matrix[i][j] * matrix[i][j + 1] *
//                           matrix[i + 1][j + 1] * matrix[i + 1][j] * matrix[i][j - 1] * matrix[i + 1][j - 1] *
//                           matrix[i - 1][j + 1];
//                matrixNew[i][j] =(int) pow(x, y);
//            } else {
//                matrixNew[i][j] = matrix[i][j];
//            }
//        }
//    }
//    Time_off = MPI_Wtime(); //timer on
//
//    printMatrix(matrixNew,"./serial_output.pgm");
//    for (int j = 0; j < hight+1; j++) {
//        free(matrixNew[j]);
//    }
//    free(matrixNew);
//    return (Time_off-Time_on);
return 0.0;
//}
//void convertToRow(double* lst, double**matrix, int start, int end){
//    int index = 0;
//    for(int i =start; i<end;++i){
//        for(int j = 0; j<N;j++){
//            lst[j+index*N]= matrix[i][j];
//        }
//        index++;
//    }
//    for(int h=0;h<N*(end-start);h++){
//        printf("%lf ",lst[h]);
//    }
//    printf("\n");
//
//
//}
}
void convertToRow(double* lst, double*matrix, int start, int end){
    int index = 0;
//    printf("GGGGGGGG\n");
//    printMatrix(matrix);
//    printf("GGGGGGGG %lf\n",matrix[(start)*N ]);
//    printf("MAT %d\n",start*N);
//
//    printf("0 %lf\n",matrix[0 ]);
//    printf("1 %lf\n",matrix[1 ]);
//    printf("2 %lf\n",matrix[2 ]);
//    printf("3 %lf\n",matrix[3 ]);
//    printf("4 %lf\n",matrix[4 ]);
//    printf("5 %lf\n",matrix[5 ]);
//    printf("6 %lf\n",matrix[6 ]);
//    printf("7 %lf\n",matrix[7 ]);
//    printf("8 %lf\n",matrix[8]);

//    for(int  i = 0; i<  N *N ; i++) {

//            printf("%lf ", matrix[i]);
//        }
//    printf("start %d\n",start);
    for(int i =start; i<end;++i){
        for(int j = 0; j<N;j++){
            lst[j+index*N]= matrix[j+(i)*N];
//            printf("matrix :%lf  i=%d j =%d\n",matrix[j+i*N],i*N,j);

        }
        index++;
    }
//    for(int h=0;h<N*(end-start);h++){
//        printf("HERE %lf ",lst[h]);
//    }
//    printf("\n");


}
int main(int argc, char * argv[]){

//    double** A = (double**)(malloc(sizeof(double*) * N));
//    for(int i=0; i < N; i++){
//        A[i] = (double *)(malloc(sizeof(double)*N));
//    }
//    double** B = (double**)(malloc(sizeof(double*) * N));
//    for(int i=0; i < N; i++){
//        B[i] = (double*)(malloc(sizeof(double)*N));
//    }
//    double** C = (double**)(malloc(sizeof(double*) * N));
//    for(int i=0; i < N; i++){
//        C[i] = (double*)(malloc(sizeof(double)*N));
//    }
    double *A = (double *)(malloc(sizeof(double)*N*N));
    double *B = (double *)(malloc(sizeof(double)*N*N));
    double *C = (double *)(malloc(sizeof(double)*N*N));
    double *X = (double *)(malloc(sizeof(double)*N*N));

//    double** X = (double**)(malloc(sizeof(double*) * N));
//    for(int i=0; i < N; i++){
//        X[i] = (double*)(malloc(sizeof(double)*N));
//    }
    double* lst = (double*)malloc(sizeof(double)*N*N);

    double Time_on, Time_off,Time_serial;
    int  numtasks,numworkers, rank, len, rc,rowNum,limit,extra,rows;
    char hostname[MPI_MAX_PROCESSOR_NAME];
    srand ( time(NULL) );
    // initialize MPI
    MPI_Init(&argc,&argv);
    // get number of tasks
    MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
    // get my rank
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    // this one is obvious
    MPI_Get_processor_name(hostname, &len);
//    printf ("Number of tasks= %d My rank= %d Running on %s\n", numtasks,rank,hostname);

    MPI_Status status;

    numworkers = numtasks-1;
//    printf("Num of workers %d\n", numworkers);

/********Master Task*********/
    if( rank == 0 ) {
        fillMatrix(A);
//        printf("Matrix A\n");
//     printMatrix(A);
//        printf("Matrix B\n");
        fillMatrix(B);
//        printMatrix(B);

//        Time_serial = serialComputing(matrix);

        rowNum = N / numworkers;
//        printf("row per worker %d\n",rowNum);
        extra = N % numworkers;
//        printf("extra for last worker %d\n",extra);
        /* send tasks to workers*/
//        Time_on = MPI_Wtime(); //timer on
        for (int dest = 1; dest <= numworkers; ++dest) {
            if (dest != numworkers) {
                rows = (dest - 1) * rowNum;
                limit = rows + rowNum;

            } else {
                rows = (dest - 1) * rowNum;
                limit = rows + rowNum + extra;
            }
//            double* lst = (double*)malloc(sizeof(double)*N*(limit-rows+1));
            convertToRow(lst, A,rows,limit);
//            printf("\n dest = %d\n", dest);
            MPI_Send(&rows, 1, MPI_INT, dest, fromMaster + 1, MPI_COMM_WORLD);
//            printf("Send from source to worker%d -  row num:%d\n",dest,rows);
            MPI_Send(&limit, 1, MPI_INT, dest, fromMaster + 2, MPI_COMM_WORLD);
//            printf("Send from source to worker%d -  limit num:%d\n",dest,limit);
            MPI_Send(&lst[0], N *(limit-rows), MPI_DOUBLE, dest, fromMaster + 3, MPI_COMM_WORLD);
//            printf("Send from source to worker%d - original matrix\n",dest);
//            printf("SEEEEMD\n");
//            printMatrix(B);
//            printf("ENDDDDD\n");

            MPI_Send(&B[0], N * N, MPI_DOUBLE, dest, fromMaster + 4, MPI_COMM_WORLD);
//            printf("Send from source to worker%d - new matrix\n",dest);
        }
        /* Receive results from workers*/
        for (int i = 1; i <= numworkers; i++) {
            int source = i;

            MPI_Recv(&rows, 1, MPI_INT, source, fromSlave + 1, MPI_COMM_WORLD, &status);
//            printf("Recv from worker%d to source -  row number:%d\n",source, rows);
            MPI_Recv(&limit, 1, MPI_INT, source, fromSlave + 2, MPI_COMM_WORLD, &status);
//           printf("Recv from worker%d to source -  limit number:%d\n",source, limit);
            MPI_Recv(&X[0], N*N, MPI_DOUBLE, source, fromSlave + 3, MPI_COMM_WORLD, &status);
//            printf("Recv from worker%d to source -  original matrix\n",source);

//            printf("Recv from worker%d to source -  new matrix\n",source);
            for (int ii = rows; ii < limit; ++ii) {
                for (int j = 0; j < N; j++) {
//                    printf("worker %d: i = %d j =%d\n",i,ii,j);

                    C[ii*N + j] = X[ii*N + j];
//                    printf("New val %lf \n",X[ii*N+j]);

                }
            }
        }

        /* Print results */
//        Time_off=MPI_Wtime();
//        printf("Parallel time with %d is :%lf\n",numworkers,Time_off - Time_on);
//        printf("Serial time with %d is :%lf\n",numworkers,Time_serial);

       printMatrix(C);

    }

    /**************************** worker task ************************************/
    if (rank > 0){
//        printf("---------------------Start receive-------------------------\n");
        MPI_Recv(&rows, 1, MPI_INT, 0, fromMaster+1, MPI_COMM_WORLD, &status);

//      printf("Recv from source to worker%d -  row number:%d\n",rank, rows);
        MPI_Recv(&limit, 1, MPI_INT, 0, fromMaster+2, MPI_COMM_WORLD, &status);
//        printf("Recv from source to worker%d -  row number:%d\n",rank, limit);
        MPI_Recv(&lst[0], N *(limit-rows), MPI_DOUBLE, 0, fromMaster+3, MPI_COMM_WORLD, &status);
//       printf("Recv from source to worker%d -  original matrix\n",rank);
        MPI_Recv(&B[0], N*N, MPI_DOUBLE, 0, fromMaster+4, MPI_COMM_WORLD, &status);
//        printf("SEEEEMD RECVVVV\n");
//        printMatrix(B);
//        printf("ENDDDDD RECCVVVV\n");
//        printf("Recv from source to worker%d -  B matrix\n",rank);
        int numOfRows = limit-rows;
//        printf("rank%d - numOfRows%d\n",rank,numOfRows);


        for(int h=0;h<N*(limit-rows);h++){
            printf("%lf|%d|  ",lst[h],rank);
        }
        printf("\n");
//        printf("Matrix B\n");

//        printMatrix(B);
//        printf("\n");

//
        double tempVar = 0.0;
        for(int indexOfRow = 0; indexOfRow < numOfRows;++indexOfRow){
            for(int i =0; i<N;++i) {
                for (int j = 0; j < N; ++j) {
//                    printf("val lst: %lf , val B: %lf\n",lst[j + indexOfRow * N],B[j*N+i]);
                   tempVar = tempVar +lst[j + indexOfRow * N] *B[j*N+i];
                printf("val lst: %lf , val B: %lf    rank %d\n",lst[j + indexOfRow * N],B[j*N+i],rank);

                }
//                printf("[%d,%d] = %lf rank:%d\n",rows+indexOfRow,i,tempVar , rank);
//                printf("%lf ",tempVar);
               X[(rows+indexOfRow)*N +i] = tempVar;
                tempVar = 0;
            }
//            printf("\n");

        }
//        printf("XXXXXXXXXXXXXXXXXXX\n");
//       printMatrix(X);
//        printf("XXXXXXXXXssssssssssssssssXXXXXXXXXX\n");


        MPI_Send(&rows, 1, MPI_INT, 0, fromSlave+1,  MPI_COMM_WORLD);
//        printf("Send from worker%d to source -  row number:%d\n",rank, rows);
        MPI_Send(&limit, 1, MPI_INT, 0, fromSlave+2,  MPI_COMM_WORLD);
//        printf("Send from worker%d to source -  limit number:%d\n",rank, limit);
//        printf("Send from worker%d to source -  original matrix \n",rank);
        MPI_Send(&X[0], N*N, MPI_DOUBLE, 0, fromSlave+3, MPI_COMM_WORLD);
//        printf("Send from worker%d to source -  new matrix \n",rank);
    }

    MPI_Finalize();



//    for (int j = 0; j < N; j++) {
//        free(A[j]);
//    }
    free(A);
//    for (int j = 0; j < N; j++) {
//        free(B[j]);
//    }
    free(B);

//    for (int j = 0; j < N; j++) {
//        free(C[j]);
//    }
    free(C);
//    for (int j = 0; j < N; j++) {
//        free(X[j]);
//    }
    free(X);
    free(lst);

    return 0;
}