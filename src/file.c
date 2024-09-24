#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int load_file(char *filepath, int **filedesc) {
    /**
    * Load and check size of the file. If the file does not exist, create it.
    */

    struct stat dbStat = {0};

    /* Get file descriptor for the file*/
    int fd = open(filepath, O_RDWR | O_CREAT, 0644);
    if (fd == -1) {
        perror("open");
        return -1;
    };

    *filedesc = malloc(sizeof(int));
    if (*filedesc == NULL) {
        perror("malloc");
        close(fd);
        return -1;
    }
    **filedesc = fd;

    // Load and check size of the file
    if (fstat(fd, &dbStat) < 0) {
        perror("fstat");
        close(fd);
        return -1;
    }
    printf("File size: %ld\n", dbStat.st_size);

    return 0; 
}