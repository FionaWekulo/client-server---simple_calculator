#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080

int main(int argc, char const* argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = { 0 };
    char* hello = "Hello from server";

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET,
                SO_REUSEADDR | SO_REUSEPORT, &opt,
                sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr*)&address,
            sizeof(address))
        < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket= accept(server_fd, (struct sockaddr*)&address,
                (socklen_t*)&addrlen))
        < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    
    
    // Receive the first integer from the client
    int num1;
    valread = read(new_socket, &num1, sizeof(num1));
    if (valread <= 0) {
        perror("read failed");
        exit(EXIT_FAILURE);
    }
    printf("Received first integer: %d\n", num1);
    
    // Receive the second integer from the client
    int num2;
    valread = read(new_socket, &num2, sizeof(num2));
    if (valread <= 0) {
        perror("read failed");
        exit(EXIT_FAILURE);
    }
    printf("Received second integer: %d\n", num2);

    int a;
    valread = read(new_socket, &a, sizeof(a));
    if (valread <= 0) {
        perror("read failed");
        exit(EXIT_FAILURE);
    }
    printf("Received second integer: %d\n", a);
    
    // Perform the selected operation and send the result to the client
    int result;
    switch (a) {
        case 1:
            result = num1 + num2;
            break;
        case 2:
            result = num1 - num2;
            break;
        case 3:
            result = num1 * num2;
            break;
        case 4:
            result = num1 / num2;
            break;
        default:
            perror("invalid operation");
            exit(EXIT_FAILURE);
    }
    printf("Result: %d\n", result);
    send(new_socket, &result, sizeof(result), 0);
    printf("Result sent to client\n");

    // closing the connected socket
    close(new_socket);
    // closing the listening socket
    shutdown(server_fd, SHUT_RDWR);
    return 0;
}
