#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <string.h>

#include "parse.h"
#include "common.h"


/* I whink file descriptor is not requried here*/
int create_db_header(struct dbHeader **p_headerOut) {
    struct  dbHeader *p_header = calloc(1, sizeof(struct dbHeader));
    if (p_header == NULL) {
        printf("Failed to allocate memory for db header\n");
        return STATUS_ERROR;
    }

    p_header->version = 0x1;
    p_header->count = 0;
    p_header->magic = HEADER_MAGIC;
    p_header->fileSize = sizeof(struct dbHeader);

    *p_headerOut = p_header;

    return STATUS_SUCCESS;
};


int validate_db_header(int fd, struct dbHeader **p_headerOut) {
    if (fd < 0) {
        printf("Got invalid file descriptor\n");
        return STATUS_ERROR;
    }

    struct dbHeader *p_header = calloc(1, sizeof(struct dbHeader));
    if (p_header == NULL) {
        printf("Failed to allocate memory for db header\n");
        return STATUS_ERROR;
    }

    if (read(fd, p_header, sizeof(struct dbHeader)) != sizeof(struct dbHeader)) {
        perror("read");
        free(p_header);
        return STATUS_ERROR;
    }

    /*Convert endianness of the header*/
    p_header->version = ntohs(p_header->version);
    p_header->count = ntohs(p_header->count); 
    p_header->magic = ntohl(p_header->magic);
    p_header->fileSize = ntohl(p_header->fileSize); 

    if (p_header->magic != HEADER_MAGIC) {
        printf("Invalid magic number\n");
        free(p_header);
        return STATUS_ERROR;
    }

    if (p_header->version != 0x1) {
        printf("Invalid version number\n");
        free(p_header);
        return STATUS_ERROR;
    }

    struct stat dbStat = {0};
    fstat(fd, &dbStat);
    if (dbStat.st_size != p_header->fileSize) {
        printf("Invalid file size, corrupted ddbb!\n");
        free(p_header);
        return STATUS_ERROR;
    }

    *p_headerOut = p_header;
    return STATUS_SUCCESS;
}


int output_file(int fd, struct dbHeader *p_header, struct employee *p_employees) {
    if (fd < 0) {
        printf("Got invalid file descriptor\n");
        return STATUS_ERROR;
    }

    int count = p_header->count;
    int fileSize = sizeof(struct dbHeader) + count * sizeof(struct employee);

    /*Convert endianness of the header*/
    p_header->version = htons(p_header->version);
    p_header->count = htons(p_header->count); 
    p_header->magic = htonl(p_header->magic);
    p_header->fileSize = htonl(fileSize); 
    
    ftruncate(fd, fileSize);
    /* Bring the cursor in the file the begining of the file*/
    lseek(fd, 0, SEEK_SET);
    write(fd, p_header, sizeof(struct dbHeader));

    int i = 0;
    for (; i < count; i++) {
        p_employees[i].hours = htonl(p_employees[i].hours);
        write(fd, &p_employees[i], sizeof(struct employee));
    }

    return STATUS_SUCCESS;
}


int read_employees(int fd, struct dbHeader *p_header, struct employee **p_employeesOut) {
    if (fd < 0) {
        printf("Got invalid file descriptor\n");
        return STATUS_ERROR;
    }

    int count = p_header->count;
    struct employee *p_employees = calloc(count, sizeof(struct employee));
    if (p_employees == NULL) {
        printf("Failed to allocate memory for employees\n");
        return STATUS_ERROR;
    }

    if (read(fd, p_employees, count * sizeof(struct employee)) == -1) {
        perror("read");
        free(p_employees);
        return STATUS_ERROR;
    }


    int i = 0;
    for (; i < count; i++) {
        /*Convert endianness of the employees*/
        p_employees[i].hours = ntohl(p_employees[i].hours);
    }

    *p_employeesOut = p_employees;
    return STATUS_SUCCESS;
}


int add_employee(struct dbHeader *p_header, struct employee *p_employees, char *p_addString) {

    char *p_name = strtok(p_addString, ",");
    char *p_address = strtok(NULL, ",");
    char *p_hours = strtok(NULL, ",");


    strncpy(
        p_employees[p_header->count-1].name,
        p_name,
        sizeof(p_employees[p_header->count-1].name)
    );
    strncpy(
        p_employees[p_header->count-1].adress,
        p_address,
        sizeof(p_employees[p_header->count-1].adress)
    );
    p_employees[p_header->count-1].hours = atoi(p_hours);
    
    return STATUS_SUCCESS;
}


int remove_employee(struct dbHeader **p_header, struct employee **p_employees, char *p_removeString) {
    int i = 0;
    int matchCount = 0;
    int employeeIndex = -1;

    for (; i < (*p_header)->count; i++) {
        if (strcmp((*p_employees)[i].name, p_removeString) == 0) {
            matchCount++;
        }
    }

    for (int j=0; j<matchCount; j++) {
        for (i=0; i < (*p_header)->count; i++) {
            if (strcmp((*p_employees)[i].name, p_removeString) == 0) {
                for (int k = i; k < (*p_header)->count - 1; k++) {
                    (*p_employees)[k] = (*p_employees)[k+1];
                }
                break;
            }
        }
    }
    (*p_header)->count = (*p_header)->count - matchCount;


    struct employee *temp = realloc(*p_employees, (*p_header)->count * sizeof(struct employee));
    if (temp == NULL) {
        printf("Failed to reallocate memory for employees\n");
        return STATUS_ERROR;
    }
    *p_employees = temp;

    return STATUS_SUCCESS;
}


void list_employees(struct dbHeader *p_header, struct employee *p_employees) {
    printf("Current employees, in ddbb:\n");
    int i = 0;
    for (; i < p_header->count; i++) {
        printf("%s, %s, %d\n", p_employees[i].name, p_employees[i].adress, p_employees[i].hours);
    }
    return;
}