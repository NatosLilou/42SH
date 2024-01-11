#include "io_back_end.h"

#include <stdio.h>

int main(int argc, char* argv[])
{
    char *string = io_back_end(argc, argv);
    printf("%s", string);
    free(string);
    return 0;
}