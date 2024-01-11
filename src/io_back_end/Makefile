CC=gcc
CFLAGS= -Wall -Werror -Wextra -Wvla -std=c99 -pedantic

OBJS = io_back_end.o\
	main.o

BIN = main

all : $(BIN)

$(BIN) : $(OBJS)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJS)

check : $(OBJS)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJS) -fsanitize=address -g
	./test.sh

clean :
	$(RM) $(BIN) $(OBJS) output.txt expected_output.txt