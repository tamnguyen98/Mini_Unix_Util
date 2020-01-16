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
void strcpy_fix (char *to, char *from);



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
        strcpy_fix(file_name, argv[i]);
        FILE *fp;

        if ((fp = fopen(file_name, "r+")) == NULL)
        {
            // If there is an error opening file
            fprintf(stderr, "%s: %s\n", strerror(errno), file_name);
            fclose(fp);
            return errno;
        }

        parse_file(fp, look_for, replace_with);
        fclose(fp);

        if (rename(tmp_file_name, file_name) != 0)
        {
            // If there is an error renaming the file.
            fprintf(stderr, "%s\n", strerror(errno));
            return errno;
        }
    }
    
}

void parse_file(FILE *fp, char *look_for, char *replace_with)
{
    char *lineptr;
    int lfLen = strlen(look_for);
    char rwLen = strlen(replace_with);
    size_t n;
    ssize_t read_count;
    FILE *tmp_file = fopen(tmp_file_name, "w");
    
    while ((read_count = getline(&lineptr, &n, fp)) > 0)
    {
        if (strlen(lineptr) >= strlen (look_for))
        {
            // Split the line by space.
            char *chunk = strtok(lineptr, " ");
            while(chunk != NULL)
            {
                int chunk_len = strlen(chunk);

                // Process each word
                if (strncmp(chunk,look_for, lfLen) == 0  && (chunk_len <= lfLen+1 && chunk_len >= lfLen))
                {
                    // Write new word to a different file to act as replace feature
                    fputs(replace_with, tmp_file);

                    if (chunk[chunk_len-1] == '\n')
                        fputs("\n", tmp_file);
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


// Had to use this function instead of strcpy, because strcpy (for some reason)
// keep changing argv[3] value once it get process by the parse function above
void strcpy_fix (char *to, char *from)
{
    int i;
    for (i = 0; from[i] != '\0' && from[i] != '\n'; i++)
        to[i] = from[i];
    to[i] = '\0';
}
