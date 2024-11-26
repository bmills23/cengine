CC = gcc
CFLAGS = -Wall -g
TARGET = index

$(TARGET): index.c
	$(CC) $(CFLAGS) index.c -o $(TARGET)

clean:
	rm -f $(TARGET)