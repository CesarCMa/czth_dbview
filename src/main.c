#include <stdio.h>
#include <stdbool.h>
#include <getopt.h>

#include "file.h"
#include "common.h"

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
    int dbFileDesc = -1;
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

    if (p_filePath == NULL) {
        printf("Filepath is a required argument \n");
        print_usage(argv);

        return 0;
    }

    printf("Newfile: %d\n", newFile);
    printf("p_filePath: %s\n", p_filePath);
    
    if (newFile) {
        dbFileDesc = create_db_file(p_filePath);
        if (dbFileDesc == STATUS_ERROR) {
            printf("Unable to create new db file\n");
            return -1;
        }
    } else {
        dbFileDesc = open_db_file(p_filePath);
        if (dbFileDesc == STATUS_ERROR) {
            printf("Unable to open db file\n");
            return -1;
        }
    }


    printf("File descriptor: %d\n", dbFileDesc);

    return 0;
}