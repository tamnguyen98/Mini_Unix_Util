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

int main(int argc, char **argv)
{
    if (argc <= 3)
    {
        fprintf(stderr, "my-sed: find term replace term [file ...]\n");
        return 1;
    }

    look_for = argv[1];
    replace_with = argv[2];

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
    
    while ((read_count = getline(&lineptr, &n, fp)) > 0)
    {
        printf("> %s", lineptr);
        if (strlen(lineptr) >= strlen (look_for))
        {
            char *chunk = strtok(lineptr, " ");
            printf("-----------------------\n");
            while(chunk != NULL)
            {
                if (chunk[n-2] == '\n' || chunk[n-2] == '\r') // doesn't remove new line
                {
                    chunk[n-2] = '\0';
                    printf("NEW LINE FIND");
                }
                printf("%s%c(%d)< ", chunk, chunk[n-1], (int)strlen(chunk));
                if (strcmp(chunk,look_for) == 0) // doesnt work when token has new line
                {
                    printf("Found! %s:%s\n", chunk,look_for);
                }
                chunk = strtok(NULL, " ");
            }
        }
    }

}