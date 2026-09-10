CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -Isrc
SRC = src/main.c src/core/socket.c src/http/http_request.c src/http/http_response.c src/router/router.c
OBJ = $(SRC:.c=.o)
TARGET = server

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean

