CC = gcc
CFLAGS = -Wall -g
TARGET = index
SERVER = server

$(TARGET): index.c
	$(CC) $(CFLAGS) index.c -o $(TARGET)

$(SERVER): server.c
	$(CC) $(CFLAGS) server.c -o $(SERVER) -lws2_32

clean:
	rm -f $(TARGET)