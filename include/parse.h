#ifndef PARSE_H
#define PARSE_H

/*LLAD code in hex, the magic number tells the parser if it is allowed to deal with this file*/
#define HEADER_MAGIC 0x4c4c4144

struct dbHeader {
    unsigned int magic;
    unsigned short version;
    unsigned short count;
    unsigned int fileSize;
};

int create_db_header(int fd, struct dbHeader **p_headerOut);
int validate_db_header(int fd, struct dbHeader **p_headerOut);
int output_file(int fd, struct dbHeader *p_header);


#endif