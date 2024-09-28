#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>

#include "file.h"
#include "parse.h"
#include "common.h"

void print_usage(char *argv[]) {
    printf("Usage: %s -n -f <database file path> -a <name,adress,hours> -l\n", argv[0]);
    printf("\t -n Create new database file\n");
    printf("\t -f <database file path> (required) Path to database file\n");
    printf("\t -a <comma separated string> Add new employee string with format: <name>,<address>,<hours>\n");
    printf("\t -l List all employees\n");
    printf("\t -t <name> Given the name of an employee, remove her/him from ddbb\n");
    printf("\t -u <comma separated string> Update hours for employee, given his/her name: <name>,<hours>\n");
    return;
};


int main(int argc, char *argv[]) {

    int c;
    bool newFile = false;
    bool listEmployees = false;
    char *p_filePath = NULL;
    char *p_addString = NULL;
    char *p_removeString = NULL;	
    char *p_updateString = NULL;	
    int dbFileDesc = -1;
    struct dbHeader *p_header = NULL;
    struct employee *p_employees = NULL;
    


    while ((c = getopt(argc, argv, "nf:a:lr:u:")) != -1) { 
        switch (c) {
            case 'n':
                newFile = true;
                break;
            case 'f':
                p_filePath = optarg;
                break;
            case 'a':
                p_addString = optarg;
                break;
            case 'l':
                listEmployees = true;
                break;
            case 'r':
                p_removeString = optarg;
                break;
            case 'u':
                p_updateString = optarg;
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

    
    if (newFile) {
        dbFileDesc = create_db_file(p_filePath);
        if (dbFileDesc == STATUS_ERROR) {
            printf("Unable to create new db file\n");
            return -1;
        }

        if (create_db_header(&p_header) == STATUS_ERROR) {
            printf("Failed to create db header\n");
            return -1;
        }

    } else {
        dbFileDesc = open_db_file(p_filePath);
        if (dbFileDesc == STATUS_ERROR) {
            printf("Unable to open db file\n");
            return -1;
        }

        if (validate_db_header(dbFileDesc, &p_header) == STATUS_ERROR) {
            printf("Failed to validate db header\n");
            return -1;
        }
    }

    if (read_employees(dbFileDesc, p_header, &p_employees) == STATUS_ERROR) {
        printf("Failed to read employees\n");
        return -1;
    }

    if (p_addString) {
        p_header->count++;
        p_employees = realloc(p_employees, p_header->count * sizeof(struct employee));
        if (p_employees == NULL) {
            printf("Failed to reallocate memory for new employe record\n");
            free(p_employees);
            return -1;
        }

        if (add_employee(p_header, p_employees, p_addString) == STATUS_ERROR) {
            printf("Failed to add employee\n");
            return -1;
        }
    }

    if (p_updateString) {
        printf("Updating employee: %s\n", p_updateString);
        if (update_employee(p_header, p_employees, p_updateString) == STATUS_ERROR) {
            printf("Failed to update employee\n");
            return -1;
        }
    }

    if (p_removeString) {
        if (remove_employee(&p_header, &p_employees, p_updateString) == STATUS_ERROR) {
            printf("Failed to remove employee\n");
            return -1;
        }
    }

    if (listEmployees) {
        list_employees(p_header, p_employees);
    }

    if (output_file(dbFileDesc, p_header, p_employees) == STATUS_ERROR) {
        printf("Failed to output file\n");
        return -1;
    }

    return 0;
}