#include "io_back_end.h"

FILE *io_back_end_init(int argc, char *argv[])
{
    if (argc == 1)
    {
        return stdin; // /!\ DONT FORGET TO FCLOSE
    }
    size_t len = strlen(argv[1]);
    if (argc == 2 && len > 3 && argv[1][len - 1] == 'h'
        && argv[1][len - 2] == 's' && argv[1][len - 3] == '.')
    {
        FILE *stream = fopen(argv[1], "r"); // /!\ DONT FORGET TO FCLOSE FD
        if (stream == NULL)
            goto erro;
        return stream;
    }
    if (argc == 3 && strcmp(argv[1], "-c") == 0)
    {
        FILE *stream = fmemopen(argv[2], strlen(argv[2]), "r");
        return stream;
    }

erro:
    printf("42sh [OPTIONS] [SCRIPT] [ARGUMENTS ...]");
    return NULL;
}

char io_back_end_read(FILE *stream, long offset)
{
    fseek(stream, offset, SEEK_SET);
    char res = 0;
    if (fread(&res, sizeof(char), 1, stream) == 0)
        return '\0';
    return res;
}

void io_back_end_close(FILE *stream)
{
    fclose(stream);
}
/*
char *io_back_end(int argc, char *argv[])
{
    if (argc == 1)
    {
        ssize_t read_bytes = 0;
        size_t size_res = 0;
        char *buffer = calloc(1024, sizeof(char));
        char *res = calloc(1, sizeof(char));
        while ((read_bytes = read(STDIN_FILENO, buffer ,1024)) != 0)
        {
            res = realloc(res, (size_res + read_bytes + 1) * sizeof(char));
            for (size_t i = 0; i < (size_t)read_bytes; i++)
            {
                res[size_res++] = buffer[i];
            }
        }
        res[size_res] = 0;
        free(buffer);
        return res;
    }
    if (argc == 3 && strcmp(argv[1], "-c") == 0)
    {
        char *res = calloc(strlen(argv[2]) + 1, sizeof(char));
        memcpy(res, (char *)argv[2], strlen(argv[2]));
        return res;
    }
    size_t len = strlen(argv[1]);
    if (argc == 2 && len > 3 && argv[1][len - 1] == 'h'
        && argv[1][len - 2] == 's' && argv[1][len - 3] == '.')
    {
        int fd = open(argv[1], O_RDONLY);
        ssize_t read_bytes = 0;
        size_t size_res = 0;
        char *buffer = calloc(1024, sizeof(char));
        char *res = calloc(1, sizeof(char));
        while ((read_bytes = read(fd, buffer, 1024)) != 0)
        {
            res = realloc(res, (size_res + read_bytes + 1) * sizeof(char));
            for (size_t i = 0; i < (size_t)read_bytes; i++)
            {
                res[size_res++] = buffer[i];
            }
        }
        close(fd);
        res[size_res] = 0;
        free(buffer);
        return res;
    }
    return NULL;
}*/
