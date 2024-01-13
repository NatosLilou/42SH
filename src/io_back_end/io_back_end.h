#ifndef IO_BACK_END_H
#define IO_BACK_END_H

#define _POSIX_C_SOURCE 200809L

#include <fcntl.h>
#include <fnmatch.h>
#include <unistd.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

FILE *io_back_end_init(int argc, char *argv[]);
char io_back_end_read(FILE *stream, long offset);
void io_back_end_close(FILE *stream);
//char *io_back_end(int argc, char *argv[]);

#endif /* !IO_BACK_END_H */