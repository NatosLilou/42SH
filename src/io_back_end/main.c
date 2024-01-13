#include "io_back_end.h"

#include <stdio.h>

int main(int argc, char* argv[])
{
    FILE *stream = io_back_end_init(argc, argv);
    char test = 'a';
    for (long i = 0; test; i++)
    {
        printf("%c", io_back_end_read(stream, i));
    }
    io_back_end_close(stream);
    return 0;
}