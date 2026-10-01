CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude

TARGET = bin/shellforge

SRC = src/main.c \
      src/input.c \
      src/parser.c \
      src/process.c \
      src/builtin.c \
      src/signals.c \
      src/pipes.c

OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	mkdir -p bin
	$(CC) $(OBJ) -o $(TARGET)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

asan:
	mkdir -p bin
	$(CC) $(CFLAGS) -fsanitize=address $(SRC) -o $(TARGET)

clean:
	rm -f src/*.o
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)
