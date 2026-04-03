CC = gcc
CFLAGS = -Iinclude
LDFLAGS = -lpthread
SRC = src/*.c
TARGET = main.exe

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

test: all
	./$(TARGET) --test

clean:
	rm -f $(TARGET)