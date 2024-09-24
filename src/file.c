#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "file.h"
#include "common.h"


int create_db_file(char *p_filePath) {

    int fd = open(p_filePath, O_RDWR);
    if (fd != -1) {
        close(fd);
        printf("File already exists. Exiting...\n");
        return STATUS_ERROR;
    };

    fd = open(p_filePath, O_RDWR | O_CREAT, 0644);
    if (fd == -1) {
        perror("open");
        return STATUS_ERROR;
    };

    return fd;
};


int open_db_file(char *p_filePath) {

    int fd = open(p_filePath, O_RDWR, 0644);
    if (fd == -1) {
        perror("open");
        return STATUS_ERROR;
    };

    return fd;
}
