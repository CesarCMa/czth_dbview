#include <stdio.h>
#include <stdbool.h>
#include <getopt.h>

#include "file.h"

void print_usage(char *argv[]) {
    printf("Usage: %s -n -f <database file path>\n", argv[0]);
    printf("\t -n Create new database file\n");
    printf("\t -f <database file path> (required) Path to database file\n");
    return;
};


int main(int argc, char *argv[]) {

    char *filepath = NULL;
    bool newfile = false;
    int c;
    int *filedesc = NULL;
    long double *filesize = NULL;


    while ((c = getopt(argc, argv, "nf:")) != -1) { 
        switch (c) {
            case 'n':
                newfile = true;
                break;
            case 'f':
                filepath = optarg;
                break;
            case '?':
                printf("Unknown option -%c\n", c);
                break;
            default:
                return -1;
        }
    }
    

    if (filepath == NULL) {
        printf("Filepath is a required argument \n");
        print_usage(argv);

        return 0;
    }
    printf("Newfile: %d\n", newfile);
    printf("Filepath: %s\n", filepath);

    if (load_file(filepath, &filedesc) < 0) {
        perror("Failed to load file");
        return -1;
    }

    printf("File descriptor: %d\n", *filedesc);

    return 0;
}