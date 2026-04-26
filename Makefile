CC = gcc

CFLAGS = -Wall -Wextra -Wpedantic -std=c11 -Iinclude
LDFLAGS =

SRC = src/main.c src/parser.c src/classifier.c src/ring.c
OBJ = $(SRC:.c=.o)
TARGET = stream-reader

.PHONY: all clean valgrind

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

valgrind: $(TARGET)
	valgrind --leak-check=full --error-exitcode=1 ./$(TARGET) samples/commercial.txt

clean:
	rm -f $(OBJ) $(TARGET)