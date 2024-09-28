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

struct employee {
    char name[256];
    char adress[256];
    unsigned int hours;
};

int create_db_header(struct dbHeader **p_headerOut);
int validate_db_header(int fd, struct dbHeader **p_headerOut);
int output_file(int fd, struct dbHeader *p_header, struct employee *p_employees);
int read_employees(int fd, struct dbHeader *p_header, struct employee **p_employeesOut);
int add_employee(struct dbHeader *p_header, struct employee *p_employees, char *p_addString);
int remove_employee(struct dbHeader **p_header, struct employee **p_employees, char *p_removeString);
void list_employees(struct dbHeader *p_header, struct employee *p_employees);


#endif