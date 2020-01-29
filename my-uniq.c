#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

void myUniq(char *filepath);
void cmdLine();

int main(int argc, char *argv[]) {

    // if no arguments are entered read from the stdin
    if (argc == 1) cmdLine();

    // as long as there is at least 1 arg loop through them all
    else if (argc > 1)
        for (int ii = 1; ii < argc; ii++)
            myUniq(argv[ii]);

    return 0;
}

void myUniq(char *filepath) {

    FILE *fp;
    char *line1 = NULL;
    char *line2 = NULL;
    size_t length1 = 0;
    size_t length2 = 0;
    ssize_t numRead = 0;

    // if the file does not exist print error and terminate program
    if ((fp = fopen(filepath, "r")) == NULL) {
        fprintf(stderr, "my-uniq: cannot open file.\n");
        exit(1);
    }

    // read the first line of the file
    while (getline(&line1, &length1, fp) != -1) {
        if (line1[strlen(line1) - 1] == '\n')   // remove /n
            line1[strcspn(line1, "\n")] = '\0';

        // continue reading each subsequent line in the file until the end is reached
        while ((numRead = getline(&line2, &length2, fp)) != -1) {
            if (line2[strlen(line2) - 1] == '\n')   // remove /n
                line2[strcspn(line2, "\n")] = '\0';

            // check to see if the length of the two lines is equal, if not they can't be the same
            if (strlen(line1) == strlen(line2)) {
                if (strcmp(line1, line2) != 0) {    // check if the two line are identical
                    fprintf(stdout, "%s\n", line1);
                    free(line1);
                    line1 = NULL;
                    line1 = strdup(line2);          // dup line2 into line1 to continue comparing it
                    free(line2);
                    line2 = NULL;
                    length1 = length2; length2 = 0;
                }
            }
            else {  // the lines are not identical so print line1, then dup line2 to line1
                fprintf(stdout, "%s\n", line1);
                free(line1);
                line1 = NULL;
                line1 = strdup(line2);
                free(line2);
                line2 = NULL;
                length1 = length2; length2 = 0;
            }
        }
        // check if getline() returned -1 and set errno
        if ((errno == EINVAL) | (errno == ENOMEM)) {
            fprintf(stderr, "%s\n", strerror(errno));
            free(line1); free(line2);
            line1 = line2 = NULL;
            fclose(fp);
            exit(1);
        }
        // free memory allocated for line2 once EOF was reached
        else if (numRead == -1) {
            free(line2);
            line2 = NULL;
        }
    }

    // check if getline() returned -1 and set errno
    if ((errno == EINVAL) | (errno == ENOMEM)) {
        fprintf(stderr, "%s\n", strerror(errno));
        free(line1); free(line2);
        line1 = line2 = NULL;
        fclose(fp);
        exit(1);
    }

    // print the final line from the file in line1 if it was not already printed
    if (line1 != NULL) {
        fprintf(stdout, "%s\n", line1);
    }

    // free remaining memory and close the file
    free(line1);
    line1 = NULL;

    if (fclose(fp) != 0) {
        fprintf(stderr, "%s", strerror(errno));
        exit(1);
    }
}

// function for reading from stdin and echoing user input
void cmdLine() {
    char *line = NULL;
    size_t length = 0;

    // loop to run forever until user terminates program
    while (1) {
        getline(&line, &length, stdin); // read the input line from stdin
        fprintf(stdout, "%s", line);    // echo input to stdout
        free(line);
        line = NULL;
        length = 0;
    }
}
