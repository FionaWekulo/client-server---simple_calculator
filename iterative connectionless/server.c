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

int main(int argc, char* argv[]) {
    struct sockaddr_in sin;
    char buf[MAX_LINE];
    int len;
    int s, new_s;
    int num1, num2, result;
    char operator;

    // Create a socket
    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
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

    // Wait for incoming requests
    while(1) {
        len = sizeof(sin);
        if ((recvfrom(s, buf, MAX_LINE, 0, (struct sockaddr *)&sin, &len)) < 0) {
            perror("recvfrom");
            exit(1);
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
                    sendto(s, buf, strlen(buf), 0, (struct sockaddr *)&sin, sizeof(sin));
                    continue;
                } else {
                    result = num1 / num2;
                }
                break;
            default:
                strcpy(buf, "Error: invalid operator\n");
                sendto(s, buf, strlen(buf), 0, (struct sockaddr *)&sin, sizeof(sin));
                continue;
        }

        // Send the result back to the client
        sprintf(buf, "%d\n", result);
        sendto(s, buf, strlen(buf), 0, (struct sockaddr *)&sin, sizeof(sin));
    }

    close(s);
    return 0;
}
