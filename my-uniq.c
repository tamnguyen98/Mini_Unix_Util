#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

void myUniq(char *filepath);
void cmdLine();

int main(int argc, char *argv[]) {

    if (argc == 1) cmdLine();

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
    int err = errno;
    ssize_t numRead = 0;

    if ((fp = fopen(filepath, "r")) == NULL) {
        fprintf(stderr, "my-uniq: cannot open file.\n");
        exit(1);
    }

    while (getline(&line1, &length1, fp) != -1) {
        if (line1[strlen(line1) - 1] == '\n')
            line1[strcspn(line1, "\n")] = '\0';

        while ((numRead = getline(&line2, &length2, fp)) != -1) {
            if (line2[strlen(line2) - 1] == '\n')
                line2[strcspn(line2, "\n")] = '\0';

            if (strlen(line1) == strlen(line2)) {
                if (strcmp(line1, line2) != 0) {
                    fprintf(stdout, "%s\n", line1);
                    free(line1);
                    line1 = NULL;
                    line1 = strdup(line2);
                    free(line2);
                    line2 = NULL;
                    length1 = length2; length2 = 0;
                }
            }
            else {
                fprintf(stdout, "%s\n", line1);
                free(line1);
                line1 = NULL;
                line1 = strdup(line2);
                free(line2);
                line2 = NULL;
                length1 = length2; length2 = 0;
            }
        }
        if (errno == EINVAL | errno == ENOMEM) {
            fprintf(stderr, "%s\n", strerror(errno));
            free(line1); free(line2);
            line1 = line2 = NULL;
            fclose(fp);
            exit(1);
        }
        else if (numRead == -1) {
            free(line2);
            line2 = NULL;
        }
    }

    if (errno == EINVAL | errno == ENOMEM) {
        fprintf(stderr, "%s\n", strerror(errno));
        free(line1); free(line2);
        line1 = line2 = NULL;
        fclose(fp);
        exit(1);
    }
    if (line1 != NULL) {
        fprintf(stdout, "%s\n", line1);
    }

    free(line1);
    line1 = NULL;
    fclose(fp);
}

void cmdLine() {
    char *line = NULL;
    size_t length = 0;

    while (1) {
        getline(&line, &length, stdin);
        fprintf(stdout, "%s", line);
        free(line);
        line = NULL;
        length = 0;
    }
}
