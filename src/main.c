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

    char *p_filePath = NULL;
    bool newFile = false;
    int c;
    int *p_dbFileDesc = NULL;
    long double *p_fileSize = NULL;


    while ((c = getopt(argc, argv, "nf:")) != -1) { 
        switch (c) {
            case 'n':
                newFile = true;
                break;
            case 'f':
                p_filePath = optarg;
                break;
            case '?':
                printf("Unknown option -%c\n", c);
                break;
            default:
                return -1;
        }
    }
    
    if (newFile) {
        
    }

    if (p_filePath == NULL) {
        printf("Filepath is a required argument \n");
        print_usage(argv);

        return 0;
    }
    printf("Newfile: %d\n", newFile);
    printf("p_filePath: %s\n", p_filePath);

    if (load_file(p_filePath, &p_dbFileDesc) < 0) {
        perror("Failed to load file");
        return -1;
    }

    printf("File descriptor: %d\n", *p_dbFileDesc);

    return 0;
}