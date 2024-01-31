#include "io_back_end.h"

extern struct assigned_var *assigned;

struct io *io_back_end_init(int argc, char *argv[])
{
    struct io *io = calloc(1, sizeof(struct io));
    io->stream = NULL;
    io->buffer = '\0';

    if (argc == 1)
    {
        // printf("READ IN STDIN\n");
        io->isatty = isatty(STDIN_FILENO);
        io->stream = stdin;
        return io;
    }
    if (argc == 2)
    {
        // printf("READ IN FILE\n");
        FILE *stream = fopen(argv[1], "r"); // /!\ DONT FORGET TO FCLOSE FD
        if (stream == NULL)
            goto erro;
        io->stream = stream;
        return io;
    }
    if (argc == 3 && strcmp(argv[1], "-c") == 0)
    {
        FILE *stream = fmemopen(argv[2], strlen(argv[2]), "r");
        io->stream = stream;
        return io;
    }
    else
    {
        FILE *stream = fopen(argv[1], "r"); // /!\ DONT FORGET TO FCLOSE FD
        if (stream == NULL)
            goto erro;
        io->stream = stream;
        for (size_t i = 2; i < (size_t)argc; i++)
        {
            assigned->args[assigned->pos_args] = argv[i];
            assigned->pos_args++;
        }
        return io;
    }

erro:
    fprintf(stderr, "42sh: Invalid syntax\n");
    fprintf(stderr, "42sh: Usage: ./42sh [OPTIONS] [SCRIPT] [ARGUMENTS ...]\n");
    free(io);
    return NULL;
}

char io_back_end_peek(struct io *io)
{
    if (io->buffer)
    {
        // printf("%c", io->buffer);
        return io->buffer;
    }

    io->buffer = getc(io->stream);
    if (io->buffer == '\n')
    {
        fflush(io->stream);
    }
    if (io->buffer == -1)
    {
        // printf("IO EOF\n");
        return '\0';
    }
    return io->buffer;
}

char io_back_end_pop(struct io *io)
{
    if (io->buffer)
    {
        char c = io->buffer;
        io->buffer = '\0';
        return c;
    }

    return getc(io->stream);
}

void io_back_end_close(struct io *io)
{
    fclose(io->stream);
    free(io);
}
