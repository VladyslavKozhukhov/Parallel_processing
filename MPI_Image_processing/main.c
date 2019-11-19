#include <stdio.h>
#include <mpi/mpi.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char * argv[]){
//    MPI_Init(&argc, &argv);
//  ?  MPI_Finalize();
    if(argc  == 2 ){
        FILE* file = fopen(argv[1], "rb");
        FILE* output = fopen("/home/vladko/CLionProjects/Mpi_hw1/test.jpg","wb");
        fseek(file, 0, SEEK_END);
        int fileLen=ftell(file);
        char* file_data;
        rewind(file);
        file_data=(char*)malloc((fileLen + 1)*sizeof(char));

        if (file_data == NULL){
            printf("Memory error"); exit (2);
        }
        int num_read=0;
        char s;
        printf("size of file %u",fileLen);
        while ((num_read = fread(&s, 1, 1, file))) {
            strncat(file_data,&s,1);
            fprintf(output,"%c",s);

            printf("file contents: %s", file_data);

        }
        fclose(output);
        fclose(file);

    }


return 0;
}