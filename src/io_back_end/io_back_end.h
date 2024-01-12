#ifndef IO_BACK_END_H
#define IO_BACK_END_H

#include <fcntl.h>
#include <fnmatch.h>
#include <unistd.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>


char *io_back_end(int argc, char *argv[]);

#endif /* !IO_BACK_END_H */
