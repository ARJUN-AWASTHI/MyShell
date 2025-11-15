CC = gcc
CFLAGS = -Wall
LDFLAGS = -lreadline -lhistory -lncurses

SRC = main.c utils.c builtin.c
OBJ = $(patsubst %.c,obj/%.o,$(SRC))

main: $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o main

obj/%.o: %.c shell.h | obj
	$(CC) $(CFLAGS) -c $< -o $@

obj:
	mkdir -p obj

clean:
	rm -f obj/*.o main

.PHONY: clean
