#ifndef IO_BACK_END_H
#define IO_BACK_END_H

#include <fcntl.h>
#include <fnmatch.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

char *io_back_end(int argc, char *argv[]);

#endif /* !IO_BACK_END_H */
