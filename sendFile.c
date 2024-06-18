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
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <fileName> <server-IP-address:port-number> [bufSize]\n", argv[0]);
        return 1;
    }

    char *fileName = argv[1];
    char *serverIP = strtok(argv[2], ":");
    char *portStr = strtok(NULL, ":");
    int bufSize = (argc > 3) ? atoi(argv[3]) : DEFAULT_BUF_SIZE;

    FILE *file = fopen(fileName, "r");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    if (serverIP == NULL || portStr == NULL) {
        fprintf(stderr, "Invalid IP address and port format\n");
        return 1;
    }

    int serverPort = atoi(portStr);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Error creating socket");
        fclose(file);
        return 1;
    }

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(serverPort);
    inet_pton(AF_INET, serverIP, &serverAddress.sin_addr);

    if (connect(sock, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Error connecting to server");
        fprintf(stderr, "Error code: %d\n", errno);
        close(sock);
        fclose(file);
        return 1;
    }

    // Send filename first
    send(sock, fileName, strlen(fileName), 0);
    send(sock, "\n", 1, 0);  // Using newline as delimiter for simplicity

    // Send a delimiter
    char delimiter = '\0'; // Using null terminator as delimiter
    send(sock, &delimiter, 1, 0);

    // send the file content in chunks
    char buffer[bufSize];
    size_t bytesRead;
    while ((bytesRead = fread(buffer, 1, bufSize, file)) > 0) {
        ssize_t bytesSent = 0;
        while (bytesSent < bytesRead) {  // Ensure all read bytes are sent
            ssize_t result = send(sock, buffer + bytesSent, bytesRead - bytesSent, 0);
            if (result < 0) {
                perror("Error sending file");
                fprintf(stderr, "Error code: %d\n", errno);
                close(sock);
                fclose(file);
                return 1;
        }
        bytesSent += result;
    }
}
    close(sock);
    fclose(file);
    printf("File sent successfully.\n");
    return 0;
}
