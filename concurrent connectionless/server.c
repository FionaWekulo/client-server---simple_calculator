#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_PORT 8000
#define MAX_LINE 1024

void handle_client(int new_s) {
    char buf[MAX_LINE];
    int num1, num2, result;
    char operator;

    while (1) {
        // Receive data from client
        memset(buf, 0, sizeof(buf));
        int n = read(new_s, buf, sizeof(buf));
        if (n <= 0) {
            break;
        }

        // Parse the input string
        sscanf(buf, "%d%c%d", &num1, &operator, &num2);

        // Perform the requested operation
        switch(operator) {
            case '+':
                result = num1 + num2;
                break;
            case '-':
                result = num1 - num2;
                break;
            case '*':
                result = num1 * num2;
                break;
            case '/':
                if (num2 == 0) {
                    strcpy(buf, "Error: division by zero\n");
                    write(new_s, buf, strlen(buf));
                    continue;
                } else {
                    result = num1 / num2;
                }
                break;
            default:
                strcpy(buf, "Error: invalid operator\n");
                write(new_s, buf, strlen(buf));
                continue;
        }

        // Send the result back to the client
        sprintf(buf, "%d\n", result);
        write(new_s, buf, strlen(buf));
    }

    // Close the client socket
    close(new_s);
}

int main(int argc, char* argv[]) {
    struct sockaddr_in sin;
    int s, new_s;

    // Create a socket
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    // Bind the socket to a local address
    bzero((char *)&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(SERVER_PORT);
    if (bind(s, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
        perror("bind");
        exit(1);
    }

    printf("Server is ready to receive...\n");

    // Listen for incoming connections
    if (listen(s, 5) < 0) {
        perror("listen");
        exit(1);
    }

    while (1) {
        // Accept a new client connection
        new_s = accept(s, NULL, NULL);
        if (new_s < 0) {
            perror("accept");
            exit(1);
        }

        // Fork a new process to handle the client
        if (fork() == 0) {
            close(s);
            handle_client(new_s);
            exit(0);
        } else {
            close(new_s);
        }
    }

    close(s);
    return 0;
}
