#ifndef IO_BACK_END_H
#define IO_BACK_END_H

#define _POSIX_C_SOURCE 200809L

#include <fcntl.h>
#include <fnmatch.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

struct io
{
    FILE *stream;
    char buffer;
    int isatty;
};

struct assigned_var
{
    char **name;
    char **value;
    size_t pos;
    char **args;
    size_t pos_args;
};

struct io *io_back_end_init(int argc, char *argv[]);
char io_back_end_peek(struct io *io);
char io_back_end_pop(struct io *io);
void io_back_end_close(struct io *io);

#endif /* !IO_BACK_END_H */
