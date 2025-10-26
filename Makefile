CC = gcc
CFLAGS = -Wall -Wextra
SRC = src/main.c src/generate.c src/lookup.c src/utils.c
OBJ = $(SRC:.c=.o)
EXEC = condensat

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(OBJ) $(EXEC)

.PHONY: all clean
