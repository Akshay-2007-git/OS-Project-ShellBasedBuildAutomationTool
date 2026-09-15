CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

TARGET = bin/shellforge

SRC = src/main.c \
      src/input.c \
      src/parser.c \
      src/process.c \
      src/builtin.c

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
