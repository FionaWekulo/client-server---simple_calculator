#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080

int main(int argc, char const* argv[])
{
	int status, valread, client_fd;
	struct sockaddr_in serv_addr;
    int op;
	int num1, num2; // two integers to send
	char buffer[1024] = { 0 };
	if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	// Convert IPv4 and IPv6 addresses from text to binary
	// form
	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)
		<= 0) {
		printf(
			"\nInvalid address/ Address not supported \n");
		return -1;
	}

	if ((status= connect(client_fd, (struct sockaddr*)&serv_addr,
				sizeof(serv_addr)))
		< 0) {
		printf("\nConnection Failed \n");
		return -1;
	}
    printf("Enter 2 numbers: \n");
    scanf("%d",&num1);
    scanf("%d",&num2);
    printf("Choose an operation to perform:\n1.Addition\n2.Subtraction\n3.Multiplication\n4.Division\n");
    scanf("%d",&op);
	send(client_fd, &num1, sizeof(num1), 0);
    send(client_fd, &num2, sizeof(num2), 0); 
    send(client_fd, &op, sizeof(op), 0);// send the two integers
	printf("Integers sent: %d, %d\n", num1, num2);
    int  result;
	valread = read(client_fd, &result, sizeof(result));
    printf("Size of result: %d bytes\n",valread);
	printf("Result: %d\n", result);

	// closing the connected socket
	close(client_fd);
	return 0;
}
