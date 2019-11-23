#include <stdio.h>
#include "mpi/mpi.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "math.h"
#include <time.h>
int width = 1920;
int hight = 1281;
#define fromSlave 2
#define fromMaster 1
char* fileName = "./leaf.pgm";

void fillMatrix(int **matrix){
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
    int flag =1;
//    printf("Opened File and ready to fill matrix\n");
    while (a != EOF & counterHight < hight)
    {
        if(counter < skip){
            if(a == newLine)
                counter ++;
        }
        else{
            if(counterWidth == width){
                counterHight++;
                counterWidth = 0;
                if( counterHight ==223){
                    int jj=0;
                }
            }
            if(isdigit(a)){
                num *= 10;
                num += a - '0';
                flag =1;
            }
            else{
                if(flag == 1) {
                    matrix[counterHight][counterWidth] = num;
                    num = 0;
                    flag =0;
                    counterWidth++;
                }
            }
        }
        a = fgetc(file);
    }
    fclose(file);
}
void printMatrix(int **matrix,char* fileName){
    FILE* fileNew = fopen(fileName, "w");
    fprintf(fileNew,"%s \n", "P2");
    fprintf(fileNew,"%d %d \n", width,hight);
    fprintf(fileNew,"%s \n", "255");
    for(int  i = 0; i<  hight  ; i++){
        for(int j = 0; j< width; j++){
            fprintf(fileNew,"%d ", matrix[i][j]);
        }
        fprintf(fileNew,"\n");
    }
    fclose(fileNew);
}
double serialComputing(int**matrix){
    int** matrixNew = (int**)(malloc(sizeof(int*)* (hight+1)));
    for(int i=0; i < hight; i++){
        matrixNew[i] = (int*)(malloc(sizeof(int)*(width+1)));
    }
    double Time_on,Time_off;
    Time_on = MPI_Wtime(); //timer on
    for(int i =0;i < hight;++i){
        for(int j = 0; j < width;++j ) {
            if (i != 0 && i != hight - 1 && j != 0 && j != width - 1) {
                double y = 1.0 / 9.0;
                double x = 1.0 * matrix[i - 1][j] * matrix[i - 1][j - 1] * matrix[i][j] * matrix[i][j + 1] *
                           matrix[i + 1][j + 1] * matrix[i + 1][j] * matrix[i][j - 1] * matrix[i + 1][j - 1] *
                           matrix[i - 1][j + 1];
                matrixNew[i][j] =(int) pow(x, y);
            } else {
                matrixNew[i][j] = matrix[i][j];
            }
        }
    }
    Time_off = MPI_Wtime(); //timer on

    printMatrix(matrixNew,"./serial_output.pgm");
    for (int j = 0; j < hight+1; j++) {
        free(matrixNew[j]);
    }
    free(matrixNew);
    return (Time_off-Time_on);
}
int main(int argc, char * argv[]){

    int** matrix = (int**)(malloc(sizeof(int*)* (hight+1)));
    for(int i=0; i < hight; i++){
        matrix[i] = (int*)(malloc(sizeof(int)*(width+1)));
    }
    double** matrixNew = (double**)(malloc(sizeof(double*)* (hight+1)));
    for(int i=0; i < hight; i++){
        matrixNew[i] = (double*)(malloc(sizeof(double)*(width+1)));
    }
    int** Res = (int**)(malloc(sizeof(int*)* (hight+1)));
    for(int i=0; i < hight; i++){
        Res[i] = (int*)(malloc(sizeof(int)*(width+1)));
    }
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
        fillMatrix(matrix);
        Time_serial = serialComputing(matrix);

        rowNum = hight / numworkers;
        extra = hight % numworkers;
        /* send tasks to workers*/
        Time_on = MPI_Wtime(); //timer on
        for (int dest = 1; dest <= numworkers; ++dest) {
            if (dest != numworkers) {
                rows = (dest - 1) * rowNum;
                limit = rows + rowNum;

            } else {
                rows = (dest - 1) * rowNum;
                limit = rows + rowNum + extra;
            }
//            printf("\n dest = %d\n", dest);
            MPI_Send(&rows, 1, MPI_INT, dest, fromMaster + 1, MPI_COMM_WORLD);
//           printf("Send from source to worker%d -  row num:%d\n",dest,rows);
            MPI_Send(&limit, 1, MPI_INT, dest, fromMaster + 2, MPI_COMM_WORLD);
//            printf("Send from source to worker%d -  limit num:%d\n",dest,limit);
            MPI_Send(&matrix[0][0], hight * width, MPI_INT, dest, fromMaster + 3, MPI_COMM_WORLD);
//            printf("Send from source to worker%d - original matrix\n",dest);
            MPI_Send(&matrixNew[0][0], hight * width, MPI_DOUBLE, dest, fromMaster + 4, MPI_COMM_WORLD);
//            printf("Send from source to worker%d - new matrix\n",dest);
        }
        /* Receive results from workers*/
        for (int i = 1; i <= numworkers; i++) {
            int source = i;
            MPI_Recv(&rows, 1, MPI_INT, source, fromSlave + 1, MPI_COMM_WORLD, &status);
//            printf("Recv from worker%d to source -  row number:%d\n",source, rows);
            MPI_Recv(&limit, 1, MPI_INT, source, fromSlave + 2, MPI_COMM_WORLD, &status);
//           printf("Recv from worker%d to source -  limit number:%d\n",source, limit);
            MPI_Recv(&matrix[0][0], hight * width, MPI_INT, source, fromSlave + 3, MPI_COMM_WORLD, &status);
//            printf("Recv from worker%d to source -  original matrix\n",source);
            MPI_Recv(&matrixNew[0][0], hight * width, MPI_DOUBLE, source, fromSlave + 4, MPI_COMM_WORLD, &status);
//            printf("Recv from worker%d to source -  new matrix\n",source);
            for (int ii = rows; ii < limit; ++ii) {
                for (int j = 0; j < width; j++) {
//                    printf("worker %d: i = %d j =%d\n",i,ii,j);
                    Res[ii][j] = (int)matrixNew[ii][j];
//                    printf("New val %lf Orig val %d\n",Res[ii][j],matrix[ii][j]);
                }
            }
        }

        /* Print results */
        Time_off=MPI_Wtime();
        printf("Parallel time with %d is :%lf\n",numworkers,Time_off - Time_on);
        printf("Serial time with %d is :%lf\n",numworkers,Time_serial);

        printMatrix(Res,"./output.pgm");

    }

    /**************************** worker task ************************************/
    if (rank > 0){
//        printf("---------------------Start receive-------------------------\n");
        MPI_Recv(&rows, 1, MPI_INT, 0, fromMaster+1, MPI_COMM_WORLD, &status);
//        printf("Recv from source to worker%d -  row number:%d\n",rank, rows);
        MPI_Recv(&limit, 1, MPI_INT, 0, fromMaster+2, MPI_COMM_WORLD, &status);
//        printf("Recv from source to worker%d -  row number:%d\n",rank, limit);
        MPI_Recv(&matrix[0][0], hight*width , MPI_INT, 0, fromMaster+3, MPI_COMM_WORLD, &status);
//        printf("Recv from source to worker%d -  original matrix\n",rank);
        MPI_Recv(&matrixNew[0][0], hight*width, MPI_DOUBLE, 0, fromMaster+4, MPI_COMM_WORLD, &status);
//        printf("Recv from source to worker%d -  new matrix\n",rank);4
        int i = rows;
        for(;i < hight;++i){
            for(int j = 0; j < width;++j ) {
                if (i >= limit) {
                    matrixNew[i][j] = 0;
                } else {
                    if (i != 0 && i != hight - 1 && j != 0 && j != width - 1) {
                        double y = 1.0 / 9.0;
                        double x = 1.0 * matrix[i - 1][j] * matrix[i - 1][j - 1] * matrix[i][j] * matrix[i][j + 1] *
                                   matrix[i + 1][j + 1] * matrix[i + 1][j] * matrix[i][j - 1] * matrix[i + 1][j - 1] *
                                   matrix[i - 1][j + 1];
                        matrixNew[i][j] = pow(x, y);
                    } else {
                        matrixNew[i][j] = matrix[i][j];
                    }
                }
            }
        }

        MPI_Send(&rows, 1, MPI_INT, 0, fromSlave+1,  MPI_COMM_WORLD);
//        printf("Send from worker%d to source -  row number:%d\n",rank, rows);
        MPI_Send(&limit, 1, MPI_INT, 0, fromSlave+2,  MPI_COMM_WORLD);
//        printf("Send from worker%d to source -  limit number:%d\n",rank, limit);
        MPI_Send(&matrix[0][0], hight*width  , MPI_INT, 0, fromSlave+3, MPI_COMM_WORLD);
//        printf("Send from worker%d to source -  original matrix \n",rank);
        MPI_Send(&matrixNew[0][0], hight*width, MPI_DOUBLE, 0, fromSlave+4, MPI_COMM_WORLD);
//        printf("Send from worker%d to source -  new matrix \n",rank);
    }

    MPI_Finalize();



    for (int j = 0; j < hight+1; j++) {
        free(matrix[j]);
    }
    free(matrix);
    for (int j = 0; j < hight+1; j++) {
        free(matrixNew[j]);
    }
    free(matrixNew);

    for (int j = 0; j < hight+1; j++) {
        free(Res[j]);
    }
    free(Res);

    return 0;
}