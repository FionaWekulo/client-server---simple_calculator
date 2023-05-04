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

int main(int argc, char* argv[]) {
    struct sockaddr_in sin;
    char buf[MAX_LINE];
    int s, len;
    char operator;
    int num1, num2;

    // Create a socket
    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    // Set up the address of the server to be contacted
    bzero ((char *)&sin, sizeof(sin));
sin.sin_family = AF_INET;
sin.sin_addr.s_addr = inet_addr(SERVER_ADDR);
sin.sin_port = htons(SERVER_PORT);
// Read the input from the user
printf("Enter a simple arithmetic operation (e.g. 2+3): ");
fgets(buf, MAX_LINE, stdin);

// Send the input to the server
len = strlen(buf);
if (sendto(s, buf, len, 0, (struct sockaddr *)&sin, sizeof(sin)) != len) {
    perror("sendto");
    exit(1);
}

// Receive the result from the server
len = sizeof(sin);
if ((recvfrom(s, buf, MAX_LINE, 0, (struct sockaddr *)&sin, &len)) < 0) {
    perror("recvfrom");
    exit(1);
}

// Print the result
printf("Result: %s", buf);

close(s);

    return 0;}
