#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void parse_file(FILE *fp, char *look_for, char *replace_with);


int main(int argc, char **argv)
{
    if (argc <= 3)
    {
        fprintf(stderr, "my-sed: find term replace term [file ...]\n");
        return 1;
    }

    char *look_for = argv[1];
    char *replace_with = argv[2];


    printf("Replace (%s) with (%s)\n", look_for, replace_with);

    FILE *fp;
    if ((fp = fopen(argv[3], "r+")) == NULL)
    {
        fprintf(stderr, "%s: %s\n", strerror(errno), argv[2]);
        fclose(fp);
        return errno;
    }

    parse_file(fp, look_for, replace_with);
    fclose(fp);

    if (rename("CS460_tMp", argv[3]) != 0)
    {
        fprintf(stderr, "%s\n", strerror(errno));
        return errno;
    }
}

void parse_file(FILE *fp, char *look_for, char *replace_with)
{
    char *lineptr;
    int lfLen = strlen(look_for);
    char rwLen = strlen(replace_with);
    size_t n;
    ssize_t read_count;
    FILE *tmp_file = fopen("CS460_tMp", "w");
    
    while ((read_count = getline(&lineptr, &n, fp)) > 0)
    {
        printf("> %s", lineptr);
        if (strlen(lineptr) >= strlen (look_for))
        {
            char *chunk = strtok(lineptr, " ");
            printf("-----------------------\n");
            while(chunk != NULL)
            {
                int chunk_len = strlen(chunk);
                if (strncmp(chunk,look_for, lfLen) == 0  && (chunk_len <= lfLen+1 && chunk_len >= lfLen)) // it just works
                {
                    printf("Found! %s:%s\n", chunk,look_for);
                    fputs(replace_with, tmp_file);
                }
                else
                    fputs(chunk, tmp_file);

                if (chunk[chunk_len-1] != '\n')
                    fputs(" ", tmp_file);
                
                chunk = strtok(NULL, " ");
            }
        }
    }
    fclose(tmp_file);
}
