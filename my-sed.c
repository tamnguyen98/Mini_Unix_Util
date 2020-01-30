#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>

/**
 * CS 460 Project 1: Make a simplified version of Unix's tool, sed.
 * But, the thing is that this program is suppose to print the FIRST line where the word occur
 * and replace only the FIRST occurance of the word in that line.
 */


void parse_file(FILE *fp, char *look_for, char *replace_with);
bool isCharNum(char c);


int main(int argc, char **argv)
{
    if (argc <= 3)
    {
        fprintf(stderr, "my-sed: find term replace term [file ...]\n");
        return 1;
    }

    char *look_for = argv[1];
    char *replace_with = argv[2];

    for (int i = 3; i < argc; i++)
    {
        // For some reason, we need to copy argv[i] to a var even though we're only using it twice
        // because if we don't the rename get messed up.
        FILE *fp;
        if ((fp = fopen(argv[i], "r+")) == NULL)
        {
            /* "• If my-sed encounters a file that it can’t open it should print “my-sed: cannot open 
                file” (followed by a newline) and exit with status code 1 immediately." */

            fprintf(stderr, "my-sed: cannot open file %s\n", argv[i]);
            exit(1);
        }

        parse_file(fp, look_for, replace_with);
        fclose(fp);
    }
    
}

void parse_file(FILE *fp, char *look_for, char *replace_with)
{
    char *lineptr = NULL;
    int lookfor_len = strlen(look_for);
    int repwit_let = strlen(replace_with);
    size_t n = 0;
    size_t read_count;
    
    while ((int)(read_count = getline(&lineptr, &n, fp)) > 0)
    {
        if (read_count >= lookfor_len)
        {
            char *found = strstr(lineptr, look_for);
            if (found != NULL)
            {
                int at = found - lineptr; // Get indx of first occurance
                bool noExtra = !isCharNum(lineptr[at+lookfor_len]); // check the end of the word if it's a valid character
                bool containsWord = false;

                if ((at == 0 && noExtra) || (!isCharNum(lineptr[at-1]) && noExtra)) // check if it's just: (example) "* word *"
                {
                    containsWord = true;
                }
                if (containsWord)
                {
                    for (int i = 0, k = 0; i < read_count; )
                    {
                        if (i >= at && i < at+lookfor_len) // If the word is in between the range
                        {
                            printf("%c", replace_with[k]);
                            k++;
                            if (k == repwit_let)
                                i = at+lookfor_len;
                        }
                        else // not on the word
                        {
                            printf("%c", lineptr[i]);
                            i++;
                        }
                    }
                    break;
                }
            }
        }
    }
    free(lineptr);
}

bool isCharNum(char c)
{
    // printf("Char '%c' is ", c);
    switch (c)
    {
        case '\0':
        case '\n':
        case '\t':
        case ' ':
        case '\r':
        case '\v':
            // printf("<");
            return false;
        default:
            // printf(">");
            return true;
    }
}
