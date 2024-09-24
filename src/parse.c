#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <arpa/inet.h>

#include "parse.h"
#include "common.h"


/* I whink file descriptor is not requried here*/
int create_db_header(int fd, struct dbHeader **p_headerOut) {
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


int output_file(int fd, struct dbHeader *p_header) {
    if (fd < 0) {
        printf("Got invalid file descriptor\n");
        return STATUS_ERROR;
    }

    /*Convert endianness of the header*/
    p_header->version = htons(p_header->version);
    p_header->count = htons(p_header->count); 
    p_header->magic = htonl(p_header->magic);
    p_header->fileSize = htonl(p_header->fileSize); 

    /* Bring the cursor in the file the begining of the file*/
    lseek(fd, 0, SEEK_SET);

    write(fd, p_header, sizeof(struct dbHeader));

    return STATUS_SUCCESS;
}