#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <errno.h>


#define DEFAULT_BUF_SIZE 4096

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <port-number> [bufSize]\n", argv[0]);
        return 1;
    }

    int port = atoi(argv[1]);
    int bufSize = (argc > 2) ? atoi(argv[2]) : DEFAULT_BUF_SIZE;

    int serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock < 0) {
        perror("Error creating socket");
        return 1;
    }

    int opt = 1;
    if (setsockopt(serverSock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
}
    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSock, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Error binding socket");
        close(serverSock);
        return 1;
    }

    printf("Socket bound to port %d\n", port);

    if (listen(serverSock, 5) < 0) {
        perror("Error listening");
        close(serverSock);
        return 1;
    }

    printf("Server listening on port %d...\n", port);

    struct sockaddr_in clientAddress;
    socklen_t clientLen = sizeof(clientAddress);

    while (1) {  // Loop to keep the server running and accepting new clients
        int clientSock = accept(serverSock, (struct sockaddr *)&clientAddress, &clientLen);
        if (clientSock < 0) {
            perror("Error accepting connection");
            continue;  // Continue listening for other clients
        }

        char buffer[bufSize];
        ssize_t bytesReceived;

        // First receive the file name
        char fileName[256];
        int index = 0;
        char c;
        while (recv(clientSock, &c, 1, 0) > 0 && c != '\0' && index < sizeof(fileName) - 1) {
            fileName[index++] = c;
        }


        if (index == 0 || index == sizeof(fileName) - 1) {
            fprintf(stderr, "Error: Invalid filename\n");
            close(clientSock);
            continue;
        }
        
        fileName[index] = '\0'; // Null-terminate the received filename

        if (strlen(fileName) == 0) {
            fprintf(stderr, "Error: No filename received\n");
            close(clientSock);
            continue;  // Continue listening for other clients
        }

        FILE *file = fopen(fileName, "w");
        if (!file) {
            perror("Error opening file");
            close(clientSock);
            continue;  // Continue listening for other clients
        }

        int chunkCount = 0;
        long totalBytes = 0;
        while ((bytesReceived = recv(clientSock, buffer, bufSize, 0)) > 0) {
            ssize_t bytesWritten = 0;
            while (bytesWritten < bytesReceived) {  // Ensure all received bytes are written
                ssize_t result = fwrite(buffer + bytesWritten, 1, bytesReceived - bytesWritten, file);
                if (result < 0) {
                    perror("Error writing to file");
                    fclose(file);
                    close(clientSock);
                    continue;  // Continue listening for other clients
        }
        bytesWritten += result;
    }

    totalBytes += bytesReceived;
    chunkCount++;
}

        printf("File received: %s, Size: %ld bytes, From: %s, Total Chunks: %d\n",
               fileName, totalBytes, inet_ntoa(clientAddress.sin_addr), chunkCount);

        fclose(file);
        close(clientSock);
        
        printf("Ready for the next client...\n");
    }

    // Clean up the server socket
    close(serverSock);
    return 0;
}
