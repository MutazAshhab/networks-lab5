#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>

#define PORT 5001
#define BUFFER_SIZE 1024

void *client_handler(void *arg)
{
    int client_fd = *(int *)arg;
    char buffer[BUFFER_SIZE];

    while (1)
    {
        // Receive a message from the client
        memset(buffer, 0, BUFFER_SIZE);
        read(client_fd, buffer, BUFFER_SIZE);
        printf("Client: %s", buffer);
    }

    close(client_fd);
    pthread_exit(NULL);
}

int main()
{
    // init variables
    int server_fd, client_fd, addr_len;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];

    // create a TCP server socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // check if socket to server has been created successfully
    if (server_fd == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // bind server address and port to `server_add` variable
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // bind the TCP socket server to the address & port
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // listen for incoming connections
    if (listen(server_fd, 1) == -1)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // accept a client connection
    addr_len = sizeof(client_addr);
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, (socklen_t *)&addr_len);
    if (client_fd == -1)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    // init variables for thread
    pthread_t thread_id;

    pthread_create(&thread_id, NULL, client_handler, (void *)&client_fd);
    pthread_detach(thread_id);

    // infinitly loop for bi-directional communication with the client
    while (1)
    {
        // Send a message to the client
        printf("Server: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        send(client_fd, buffer, strlen(buffer), 0);
    }

    // close the sockets
    close(server_fd);

    return 0;
}