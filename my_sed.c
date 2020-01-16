#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void parse_file(FILE *fp);

// Using globals since it's static
char *look_for;
char *replace_with;
int lfLen = 0;
int rwLen = 0;

int main(int argc, char **argv)
{
    if (argc <= 3)
    {
        fprintf(stderr, "my-sed: find term replace term [file ...]\n");
        return 1;
    }

    look_for = argv[1];
    replace_with = argv[2];

    lfLen = strlen(look_for);
    rwLen = strlen(replace_with);

    printf("Replace (%s) with (%s)\n", look_for, replace_with);

    FILE *fp;
    if ((fp = fopen(argv[3], "r+")) == NULL)
    {
        fprintf(stderr, "%s: %s\n", strerror(errno), argv[2]);
        fclose(fp);
        return errno;
    }
    parse_file(fp);
    fclose(fp);
}

void parse_file(FILE *fp)
{
    char *lineptr;
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
                fputs(" ", tmp_file);
                chunk = strtok(NULL, " ");
            }
        }
    }
    fclose(tmp_file);
}
