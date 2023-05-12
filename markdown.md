**Step 2**

Server.c

```c
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 5001
#define BUFFER_SIZE 1024

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

    // infinitly loop for bi-directional communication with the client
    while (1)
    {
        // Receive a message from the client
        memset(buffer, 0, BUFFER_SIZE);
        read(client_fd, buffer, BUFFER_SIZE);
        printf("Client: %s", buffer);

        // Send a message to the client
        printf("Server: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        send(client_fd, buffer, strlen(buffer), 0);
    }

    // close the sockets
    close(client_fd);
    close(server_fd);

    return 0;
}
```


Client.c
```c
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define SERVER_IP "172.23.182.56"
#define PORT 5001
#define BUFFER_SIZE 1024

int main()
{
    // init variables
    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // create a TCP client socket
    client_fd = socket(AF_INET, SOCK_STREAM, 0);

    // check if socket to client has been created successfully
    if (client_fd == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // bind server address and port to `server_add` variable
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(PORT);

    // connect the socket to the server
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    // infinitly loop for bi-directional communication with the client
    while (1)
    {
        // Send a message to the server
        printf("Client: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        send(client_fd, buffer, strlen(buffer), 0);

        // Receive a message from the server
        memset(buffer, 0, BUFFER_SIZE);
        read(client_fd, buffer, BUFFER_SIZE);
        printf("Server: %s", buffer);
    }

    // close the socket
    close(client_fd);

    return 0;
}
```