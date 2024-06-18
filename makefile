# Compiler
CC = gcc

# Compiler Flags
CFLAGS = -Wall

# Source Files
CLIENT_SRC = sendFile.c
SERVER_SRC = server.c

# Executable Files
CLIENT_EXEC = sendFile
SERVER_EXEC = server

all: $(CLIENT_EXEC) $(SERVER_EXEC)

$(CLIENT_EXEC): $(CLIENT_SRC)
	$(CC) $(CFLAGS) -o $(CLIENT_EXEC) $(CLIENT_SRC)

$(SERVER_EXEC): $(SERVER_SRC)
	$(CC) $(CFLAGS) -o $(SERVER_EXEC) $(SERVER_SRC)

clean:
	rm -f $(CLIENT_EXEC) $(SERVER_EXEC) *.o
