#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc,char* argv[]){

        FILE *fp;
        char Words[150]; //Buffer for words in each file

        if(argc < 2){ //Checking if the number of arguments are correct
                printf("Woops, please provide a file\n");
                exit(0);
        }
        for (int i = 1; i < argc; i++){ // i represents every file on command line
                fp = fopen(argv[i],"r");
                if (fp == NULL){ //If file doesn't exist
                        perror(argv[i]);
                        continue;
                }
                while (fgets(Words,150,fp) != NULL ) {
                        printf("%s",Words);
                }
                fclose(fp);
        }
}
