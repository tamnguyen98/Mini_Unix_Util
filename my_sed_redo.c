#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/**
 * CS 460 Project 1: Make a simplified version of Unix's tool, sed.
 * The main feature of this code is that it writes the new content to a new file
 * then replace it's name back to the original file after it's done parsing
 */


const char *tmp_file_name = "CS460_tMp";

void parse_file(FILE *fp, char *look_for, char *replace_with);
_Bool isCharNum(char c);


int main(int argc, char **argv)
{
    if (argc <= 3)
    {
        fprintf(stderr, "my-sed: find term replace term [file ...]\n");
        return 1;
    }

    char *look_for = argv[1];
    char *replace_with = argv[2];
    char file_name[100]; // Hopefully there are no name longer than this. Faster than recounting each file names.

    for (int i = 3; i < argc; i++)
    {
        // For some reason, we need to copy argv[i] to a var even though we're only using it twice
        // because if we don't the rename get messed up.
        strcpy(file_name, argv[i]);
        FILE *fp;
        printf("test\n");
        if ((fp = fopen(file_name, "r+")) == NULL)
        {
            // If there is an error opening file

            //PROBLEM: getting segfault when fie DNE
            fprintf(stderr, "%s: %s\n", strerror(errno), file_name);
            fclose(fp);
            if (i == argc-1)
                return errno;
            continue;
        }

        parse_file(fp, look_for, replace_with);
        fclose(fp);
    }
    
}

void parse_file(FILE *fp, char *look_for, char *replace_with)
{
    char *lineptr;
    int lfLen = strlen(look_for);
    size_t n;
    ssize_t read_count;
    
    while ((read_count = getline(&lineptr, &n, fp)) > 0)
    {
        if (n >= lfLen)
        {
            char * found = strstr(lineptr, look_for);
            if (found != NULL)
            {
                size_t at = found - lineptr;
                _Bool noExtra = !isCharNum(lineptr[at]);
                _Bool containsWord = false;
                if ((at == 0 && noExtra) || (isCharNum(lineptr[at-1]) && noExtra))
                {
                    containsWord = true;
                }
                for (int i = 0, k = 0; i < n; i++)
                {
                    if (i >= at && i < lfLen)
                    {
                        printf("%c", lfLen[k]);
                        k++;
                    }
                    else
                        printf("%c", lineptr[i]);
                }
                printf("\n");
                break;
            }
        }
    }
}

_Bool isCharNum(char c)
{
    switch (c)
    {
        case '\0':
        case '\n':
        case '\t':
        case ' ':
            return false;
        default:
            return true;
    }
    return true;
}