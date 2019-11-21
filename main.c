#include <stdio.h>
#include "mpi/mpi.h"
#include <stdlib.h>
#include <string.h>
#define _USE_MATH_DEFINES

#include "math.h"

int width =4;// 4;// 1080;
int hight = 3;//3;// 1920;
#define fromSlave 2
#define fromMaster 1
//char* fileName = "leafA.pgm";
char* fileName = "/home/vladko/CLionProjects/Mpi_hw1/test1.pgm";

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
void printMatrix(double matrix[ hight][width]){
    FILE* fileNew = fopen("/home/vladko/CLionProjects/Mpi_hw1/output.pgm", "w");
    fprintf(fileNew,"%s \n", "P2");
    fprintf(fileNew,"%s \n", "4 3");//width [4] [3]
    fprintf(fileNew,"%s \n", "255");
    for(int  i = 0; i<  hight  ; i++){
        for(int j = 0; j< width; j++){

            fprintf(fileNew,"%lf ", matrix[i][j]);
        }
        fprintf(fileNew,"\n");
    }
    fclose(fileNew);


}

int main(int argc, char * argv[]){

    //int matrix[width][hight];
    int** matrixtmp = (int**)(malloc(sizeof(int*)* hight));
    for(int i=0; i < hight; i++){
        matrixtmp[i] = (int*)(malloc(sizeof(int)*width));
    }
//    int** matrixNew = (int**)(malloc(sizeof(int*)* hight));
//    for(int i=0; i < hight; i++){
//        matrixNew[i] = (int*)(malloc(sizeof(int)*width));
//    }
    int matrix[hight][width];
    double matrixNew[hight][width];
    double Res[hight][width];

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
    printf("Num of workers %d\n", numworkers);

/********Master Task*********/
    if( rank == 0 ) {
        fillMatrix(matrixtmp);
        for(int i =0;i < hight;++i){
            for(int j = 0; j < width;++j ) {
               matrix[i][j] = matrixtmp[i][j];
                printf("%d ",  matrix[i][j]);

            }
            printf("\n");
            }
        printf("CHECK %d\n",matrix[0][3]);
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
            MPI_Send(&rows, 1, MPI_INT, dest, fromMaster+1,  MPI_COMM_WORLD);
//            printf("Send from source to worker%d -  row num:%d\n",dest,rows);
            MPI_Send(&limit, 1, MPI_INT, dest, fromMaster+2,  MPI_COMM_WORLD);
//            printf("Send from source to worker%d -  limit num:%d\n",dest,limit);
            MPI_Send(&matrix, hight*width , MPI_INT, dest, fromMaster+3, MPI_COMM_WORLD);
//            printf("Send from source to worker%d - original matrix\n",dest);
            MPI_Send(&matrixNew, hight*width, MPI_DOUBLE, dest, fromMaster+4, MPI_COMM_WORLD);
//            printf("Send from source to worker%d - new matrix\n",dest);

        }

        /* Receive results from workers*/
        for (int i = 1; i <= numworkers; i++) {
            int source = i;
            MPI_Recv(&rows, 1, MPI_INT, source, fromSlave+1, MPI_COMM_WORLD, &status);
//            printf("Recv from worker%d to source -  row number:%d\n",source, rows);
            MPI_Recv(&limit, 1, MPI_INT, source, fromSlave+2, MPI_COMM_WORLD, &status);
//            printf("Recv from worker%d to source -  limit number:%d\n",source, limit);
            MPI_Recv(matrix, hight*width , MPI_INT, source, fromSlave+3, MPI_COMM_WORLD, &status);
//            printf("Recv from worker%d to source -  original matrix\n",source);
            MPI_Recv(&matrixNew, hight*width, MPI_DOUBLE, source, fromSlave+4, MPI_COMM_WORLD, &status);
//            printf("Recv from worker%d to source -  new matrix\n",source);
            for(int ii =i-1 ;ii < limit;++ii){
                for(int j = 0; j < width;++j ) {
                    Res[ii][j] = matrixNew[ii][j];
                    printf("Enter value   %lf from wroker %d\n",matrixNew[ii][j],i);
//                    printf("Enter matrix  VAL  %d from wroker %d index %d\n",matrix[0][0],i,ii);
                }
            }
            printf("Limit %d from worker %d \n",limit,i);

        }

        /* Print results */
        printf("******************************************************\n");
        printf("Result Matrix:\n");
        printMatrix(Res);
//        printf("matrixNew  at  %lf\n",Res[1][1]);
//        printf("matrixNew  at  %lf\n",Res[1][2]);


        printf("\n******************************************************\n");
        printf("Done.\n");
    }

    /**************************** worker task ************************************/
    if (rank > 0){
        printf("---------------------Start receive-------------------------\n");
        MPI_Recv(&rows, 1, MPI_INT, 0, fromMaster+1, MPI_COMM_WORLD, &status);
//        printf("Recv from source to worker%d -  row number:%d\n",rank, rows);
        MPI_Recv(&limit, 1, MPI_INT, 0, fromMaster+2, MPI_COMM_WORLD, &status);
       // printf("Recv from source to worker%d -  row number:%d\n",rank, limit);
        MPI_Recv(matrix, hight*width , MPI_INT, 0, fromMaster+3, MPI_COMM_WORLD, &status);
//        printf("Recv from source to worker%d -  original matrix\n",rank);
        MPI_Recv(&matrixNew, hight*width, MPI_DOUBLE, 0, fromMaster+4, MPI_COMM_WORLD, &status);
//        printf("Recv from source to worker%d -  new matrix\n",rank);

//        matrix[1][1] = -1;
//        matrix[0][0] = -2;
    printf("HEEEREEEE");
    printf("HEEEREEEE\n");
        int ii = rank-1;
        printf("%d iiii\n",ii);

        for(;ii < limit;++ii){
            for(int j = 0; j < width;++j ){
                if(ii != 0 && ii != hight-1 && j !=0  && j!= width-1) {

                    printf("%d %d", matrix[ii][j], matrix[ii - 1][j]);
                }
            }
            printf("\n");
        }

        int i = rank-1;
        for(;i < limit;++i){
            for(int j = 0; j < width;++j ){
                if(i != 0 && i != hight-1 && j !=0  && j!= width-1){
                    double y = 1.0/9.0;
                    printf("%d %d %d %d %d %d %d %d %d",matrix[i-1][j]*matrix[i-1][j-1]*matrix[i][j]*matrix[i][j+1]* matrix[i+1][j+1]*matrix[i+1][j]*matrix[i][j-1]*matrix[i+1][j-1]*matrix[i-1][j+1]);
                    double x = matrix[i-1][j]*matrix[i-1][j-1]*matrix[i][j]*matrix[i][j+1]* matrix[i+1][j+1]*matrix[i+1][j]*matrix[i][j-1]*matrix[i+1][j-1]*matrix[i-1][j+1];
                    printf("x %lf and y %lf\n",x,y);
                    matrixNew[i][j]=pow(x,y);
                    printf("NEW VALUE %lf , old value %d\n",matrixNew[i][j], matrix[i][j]);
                }
                else{
                    matrixNew[i][j]=matrix[i][j];
                }
            }
        }
//       limit ++;

        MPI_Send(&rows, 1, MPI_INT, 0, fromSlave+1,  MPI_COMM_WORLD);
//        printf("Send from worker%d to source -  row number:%d\n",rank, rows);
        MPI_Send(&limit, 1, MPI_INT, 0, fromSlave+2,  MPI_COMM_WORLD);
//        printf("Send from worker%d to source -  limit number:%d\n",rank, limit);
        MPI_Send(&matrix, hight*width  , MPI_INT, 0, fromSlave+3, MPI_COMM_WORLD);
//        printf("Send from worker%d to source -  original matrix \n",rank);
        MPI_Send(&matrixNew, hight*width, MPI_DOUBLE, 0, fromSlave+4, MPI_COMM_WORLD);
//        printf("Send from worker%d to source -  new matrix \n",rank);
    }
    MPI_Finalize();


//
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