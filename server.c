#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <pthread.h>

#define MESSAGE_MAX_LENGTH 256

void* serve_client(void *arg) {
    int client_socket;
    char rx_buffer[MESSAGE_MAX_LENGTH + 1];
    char tx_buffer[MESSAGE_MAX_LENGTH + 1];
    int bytes_read = 0;
    uint8_t string_length;
    int i;
    
    client_socket = *(int *)arg;

    do {
        bytes_read += read(client_socket, rx_buffer, MESSAGE_MAX_LENGTH + 1);
    } while (bytes_read < 1);
    string_length = *(uint8_t *)rx_buffer;

    while ((bytes_read - 1) < (string_length + 1)) {
        bytes_read += read(client_socket, rx_buffer, MESSAGE_MAX_LENGTH + 1);
    }

    printf("Received:\n");
    printf("Message length: %d\n", string_length);
    printf("Message: %s\n", rx_buffer + 1);

    printf("\n");

    tx_buffer[0] = (char)string_length;
    for (i = 1; i <= string_length; ++i) {
        tx_buffer[i] = rx_buffer[(string_length + 1) - i];
    }
    tx_buffer[i] = '\0';

    printf("Sending: %s\n", tx_buffer + 1);

    write(client_socket, tx_buffer, string_length + 2);

    close(client_socket);

    free(arg);

    printf("\n---\n\n");

    return 0;
}

int main(int argc, char **argv) {
    int server_port;
    int server_socket;
    int client_socket;
    int *thread_socket;
    struct sockaddr_in server_addr;
    pthread_t thread_id;

    if (argc < 2) {
        printf("Usage: server <port>\n");
        return EXIT_FAILURE;
    }
    server_port = strtol(argv[1], NULL, 10);

    server_socket = socket(PF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        printf("Socket ERROR\n");
        return EXIT_FAILURE;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(server_port);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in)) < 0) {
        printf("Bind ERROR\n");
        return EXIT_FAILURE;
    }

    if (listen(server_socket, 4) < 0) {
        printf("Listen ERROR\n");
        return EXIT_FAILURE;
    }

    printf("Listening on port %d ...\n\n", server_port);

    while (1) {
        client_socket = accept(server_socket, NULL, NULL);
        if (client_socket >= 0) {
            thread_socket = calloc(1, sizeof(int));
            if (!thread_socket) {
                printf("Allocation ERROR\n");
                return EXIT_FAILURE;
            }
            *thread_socket = client_socket;
            pthread_create(&thread_id, NULL, &serve_client, (void *)thread_socket);
        }
        else {
            printf("Accept ERROR\n");
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}