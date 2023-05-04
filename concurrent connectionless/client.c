#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 8000
#define MAX_LINE 1024

void handle_request(int s, struct sockaddr *addr, socklen_t addrlen) {
    char buf[MAX_LINE];
    int len;

    // Read the input from the user
    printf("Enter a simple arithmetic operation (e.g. 2+3): ");
    fgets(buf, MAX_LINE, stdin);

    // Send the input to the server
    len = strlen(buf);
    if (sendto(s, buf, len, 0, addr, addrlen) != len) {
        perror("sendto");
        exit(1);
    }

    // Receive the result from the server
    len = sizeof(*addr);
    if ((recvfrom(s, buf, MAX_LINE, 0, addr, &len)) < 0) {
        perror("recvfrom");
        exit(1);
    }

    // Print the result
    printf("Result: %s", buf);
}

int main(int argc, char* argv[]) {
    struct sockaddr_in sin;
    int s;

    // Create a socket
    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    // Set up the address of the server to be contacted
    bzero((char *)&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr(SERVER_ADDR);
    sin.sin_port = htons(SERVER_PORT);

    // Fork a new process for each request
    while(1) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(1);
        } else if (pid == 0) {
            // Child process
            handle_request(s, (struct sockaddr *)&sin, sizeof(sin));
            exit(0);
        } else {
            // Parent process
            wait(NULL);
        }
    }

    close(s);
    return 0;
}
