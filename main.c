#include <stdio.h>
//#include "mpi.h"
#include <stdlib.h>
#include <string.h>

int width =225;// 1080;
int hight = 225;// 1920;
//char* fileName = "leafA.pgm";
char* fileName = "/home/vladko/CLionProjects/Mpi_hw1/leafA.pgm";

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


    printf("FFF");
    fclose(file);
}
void printMatrix(int** matrix){
    FILE* fileNew = fopen("/home/vladko/CLionProjects/Mpi_hw1/output.pgm", "w");
    fprintf(fileNew,"%s \n", "P2");
    fprintf(fileNew,"%s \n", "225 225");//width 4 3
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
    fillMatrix(matrix);
    printMatrix(matrix);
    for (int j = 0; j < hight; j++) {
        free(matrix[j]);
    }
    free(matrix);



    return 0;
}