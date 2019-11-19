#include <stdio.h>
#include "mpi/mpi.h"
#include <stdlib.h>
#include <string.h>

int width = 4;// 1080;
int hight = 3;// 1920;
//char* fileName = "leafA.pgm";
char* fileName = "/home/vladko/CLionProjects/Mpi_hw1/test1.pgm";

void fillMatrix(int** matrix){
    FILE* file = fopen(fileName, "r");
    int counter = 0;
    int skip = 3;
    char a ;
    char newLine = '\n';
    char space = ' ';
    int num = 0;
    int counterWidth = 0;
    int counterHight = 0;
    a = fgetc(file);
    printf("Opened FIle and ready to fill matrix\n");
    while (a != EOF )
    {
        if(counter < skip){
            if(a == newLine)
                counter ++;
        }
        else{
            if(a == newLine){
                matrix[counterHight][counterWidth] = num;
                counterHight++;
                counterWidth = 0;
                num = 0;
            }
            else if(a != space) {
                num *= 10;
                num += a - '0';
            }
            else{
                matrix[counterHight][counterWidth] = num;
                num = 0;
                counterWidth ++;
            }
        }
        a = fgetc(file);
    }


    fclose(file);
}
void printMatrix(int** matrix){
    FILE* fileNew = fopen("/home/vladko/CLionProjects/Mpi_hw1/output.pgm", "w");
    fprintf(fileNew,"%s \n", "P2");
    fprintf(fileNew,"%s \n", "4 3");//width 4 3
    fprintf(fileNew,"%s \n", "255");
    for(int  i = 0; i<  hight  ; i++){
        for(int j = 0; j< width; j++){

            fprintf(fileNew,"%d ", matrix[i][j]);
        }
        fprintf(fileNew,"\n");
    }
    fclose(fileNew);


}

int main(int argc, char * argv[]){

    //int matrix[width][hight];
    int** matrix = (int**)(malloc(sizeof(int*)* hight));
    for(int i=0; i < hight; i++){
        matrix[i] = (int*)(malloc(sizeof(int)*width));
    }
    int** matrixNew = (int**)(malloc(sizeof(int*)* hight));
    for(int i=0; i < hight; i++){
        matrixNew[i] = (int*)(malloc(sizeof(int)*width));
    }
    int * test = (int*)malloc(sizeof(int)*width);
 //
  //  printMatrix(matrix);

    int  numtasks,numworkers, rank, len, rc,rowNum,limit,extra,rows;
    char hostname[MPI_MAX_PROCESSOR_NAME];

    // initialize MPI
    MPI_Init(&argc,&argv);
    // get number of tasks
    MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
    // get my rank
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    // this one is obvious
    MPI_Get_processor_name(hostname, &len);
    printf ("Number of tasks= %d My rank= %d Running on %s\n", numtasks,rank,hostname);

    MPI_Status status;

    numworkers = numtasks-1;
/********Master Task*********/
    if( rank == 0 ) {
//        fillMatrix(matrix);

        rowNum = hight / numworkers;
        printf("rowNum %d\n", rowNum);
        extra = hight % numworkers;
        /* send tasks to workers*/
        for (int dest = 1; dest <= numworkers; ++dest) {
            if (dest != numworkers) {
                rows = (dest - 1) * rowNum;
                limit = rows + rowNum ;
            } else {
                rows = (dest - 1) * rowNum;
                limit = rows + rowNum + extra;
            }
            printf("FILLED MATRIX\n");
            MPI_Send(&rows, 1, MPI_INT, dest, 1,  MPI_COMM_WORLD);
            printf("Sent Row number\n");
            MPI_Send(&limit, 1, MPI_INT, dest, 1,  MPI_COMM_WORLD);
            printf("Sent limit number\n");
            MPI_Send(&matrix[0][0], width , MPI_INT, dest, 1, MPI_COMM_WORLD);
//            printf("Sent old matrix\n");
//            MPI_Send(&matrixNew, width * hight, MPI_INT, dest, 0, MPI_COMM_WORLD);
//            printf("Sent new matrix\n");

        }
        /* Receive results from workers*/
        for (int i = 1; i <= numworkers; i++) {
            int source = i;
            MPI_Recv(&rows, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
            MPI_Recv(&limit, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
           MPI_Recv(&matrix[0][0], width , MPI_INT, source, 2, MPI_COMM_WORLD, &status);
//            MPI_Recv(matrixNew, width * hight, MPI_DOUBLE, source, 2, MPI_COMM_WORLD, &status);

            printf("Received results from task %d\n", source);
        }
//        MPI_Barrier(MPI_COMM_WORLD);
        /* Print results */
        printf("******************************************************\n");
        printf("Result Matrix:\n");
//        printMatrix(matrixNew);
    printf("list  at 0 %d\n",matrix[0][0]);
        printf("\n******************************************************\n");
        printf("Done.\n");
    }
    /**************************** worker task ************************************/

    if (rank > 0){
        printf("Start receive\n");

        MPI_Recv(&rows, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        printf("Get Row number: %d\n",rows);
        MPI_Recv(&limit, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        printf("Get limit number: %d\n",limit);

        MPI_Recv(&matrix[0][0], width , MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
//        printf("Get matrix[0][0] number: %d\n",matrix[0][0]);

//        MPI_Recv(matrixNew, width * hight, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
//        for(;rows < limit ;rows ++)
//        for (int i=0; i <; k++)
//            for (i=0; i<rows; i++)
//            {
        matrix[0][0] ++;
//               / num = 0;
//               / counterWidth ++;
//              /  c[i][k] = 0.0;
//                for (j=0; j<NCA; j++)
//                    c[i][k] = c[i][k] + a[i][j] * b[j][k];
//            }
        MPI_Send(&rows, 1, MPI_INT, 0, 2,  MPI_COMM_WORLD);
        MPI_Send(&limit, 1, MPI_INT, 0, 2,  MPI_COMM_WORLD);
        MPI_Send(&matrix[0][0], width , MPI_INT, 0, 2, MPI_COMM_WORLD);
//        MPI_Send(matrixNew, width * hight, MPI_INT, 0, 2, MPI_COMM_WORLD);
    }
    MPI_Finalize();



//    for (int j = 0; j < hight; j++) {
//        free(matrix[j]);
//    }
//    free(matrix);
//
//    for (int j = 0; j < hight; j++) {
//        free(matrixNew[j]);
//    }
//    free(matrixNew);

    return 0;
}