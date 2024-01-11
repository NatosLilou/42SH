#include "io_back_end.h"

char *io_back_end(int argc, char *argv[])
{
    if (argc == 1)
    {
        ssize_t read_bytes = 0;
        size_t size_res = 1;
        char *buffer = malloc(1 * sizeof(char));
        char *res = malloc(1 * sizeof(char));
        res[0] = 0;
        while ((read_bytes = read(STDIN_FILENO, buffer ,1)) != 0)
        {
            size_res += (size_t)read_bytes;
            res = realloc(res, size_res * sizeof(char));
            res[size_res - 2] = buffer[0];
        }
        res[size_res - 1] = 0;
        free(buffer);
        return res;
    }
    if (strcmp(argv[1], "-c") == 0)
    {
        char *res = calloc(strlen(argv[2]) + 1, sizeof(char));
        memcpy(res, (char *)argv[2],strlen(argv[2]));
        return res;
    }
    size_t len = strlen(argv[1]);
    if (len > 3 && argv[1][len-1] == 'h' && argv[1][len-2] == 's' && argv[1][len-3] == '.')
    {
        int fd = open(argv[1], O_RDONLY);
        ssize_t read_bytes = 0;
        size_t size_res = 1;
        char *buffer = malloc(1 * sizeof(char));
        char *res = malloc(1 * sizeof(char));
        res[0] = 0;
        while ((read_bytes = read(fd, buffer ,1)) != 0)
        {
            size_res += (size_t)read_bytes;
            res = realloc(res, size_res * sizeof(char));
            res[size_res - 2] = buffer[0];
        }
        close(fd);
        res[size_res - 1] = 0;
        free(buffer);
        return res;
    }
    return NULL;
}
