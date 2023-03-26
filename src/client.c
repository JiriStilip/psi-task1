#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>

#define MESSAGE_MAX_LENGTH 256
#define ADDRESS_MAX_LENGTH 16

int main(int argc, char **argv) {
    int client_socket;
    struct sockaddr_in client_addr;
    char address[ADDRESS_MAX_LENGTH];
    int server_port;
    uint8_t string_length;
    char buffer[MESSAGE_MAX_LENGTH + 1];
    int bytes_read = 0;

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        printf("Socket ERROR\n");
        return EXIT_FAILURE;
    }

    if (argc < 4) {
        printf("Usage: client <IP address> <port> <message>\n");
        return EXIT_FAILURE;
    }

    strncpy(address, argv[1], ADDRESS_MAX_LENGTH);

    server_port = strtol(argv[2], NULL, 10);

    if (strlen(argv[3]) > 255) {
        printf("Message TOO LONG\n");
        return EXIT_FAILURE;
    }
    string_length = strlen(argv[3]);
    buffer[0] = (char)string_length;

    strncpy(buffer + 1, argv[3], MESSAGE_MAX_LENGTH - 1);
    buffer[MESSAGE_MAX_LENGTH] = '\0';

    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = inet_addr(address);
    client_addr.sin_port = htons(server_port);

    if (connect(client_socket, (struct sockaddr *)&client_addr, sizeof(struct sockaddr_in)) < 0) {
        printf("Connect ERROR\n");
        return EXIT_FAILURE;
    }
    printf("Connected successfully.\n\n");

    printf("Sending:\n");
    printf("Message length: %d\n", string_length);
    printf("Message: %s\n", buffer + 1);

    write(client_socket, buffer, MESSAGE_MAX_LENGTH + 1);

    printf("\n");

    memset(buffer, 0, MESSAGE_MAX_LENGTH + 1);

    do {
        bytes_read += read(client_socket, buffer, MESSAGE_MAX_LENGTH + 1);
    } while ((bytes_read - 1) < (string_length + 1));  

    printf("Received:\n");
    printf("Message length: %d\n", string_length);
    printf("Message: %s\n", buffer + 1);

    printf("\n");

    printf("Closing.\n");
    close(client_socket);

    return EXIT_SUCCESS;
}