CC = gcc

CFLAGS = -Wall -Wextra -g -Iinclude

TARGET = bin/shellforge

SRC = src/main.c \
      src/input.c \
      src/parser.c \
      src/process.c \
      src/builtin.c \
      src/signals.c \
      src/pipes.c \
      src/redirect.c

OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	mkdir -p bin
	$(CC) $(OBJ) -o $(TARGET)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.o
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

asan:
	$(CC) $(CFLAGS) -fsanitize=address -fno-omit-frame-pointer $(SRC) -o $(TARGET)
