#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 3000

int main(){

	int clientSocket, ret, num1, num2, choice;
	struct sockaddr_in serverAddr;
	char buffer[1024];

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0){
		printf("Connection error.\n");
		exit(1);
	}
	printf("Client Socket created successfully.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("Connection error.\n");
		exit(1);
	}
	printf("Connected to Server successfully.\n");

	while(1){
    printf("\n1. Addition\n2. Subtraction\n3. Multiplication\n4. Division\n5. Exit\n");
    printf("Choose operation: ");
    scanf("%d", &choice);

		if(choice == 5){
			printf("Disconnected from server.\n");
      sprintf(buffer, "%d %d %d", 0, choice, 0);
      send(clientSocket, buffer, strlen(buffer), 0);
			close(clientSocket);
			exit(1);
		}

    printf("Enter first number: ");
    scanf("%d", &num1);

    printf("Enter second number: ");
    scanf("%d", &num2);

    sprintf(buffer, "%d %d %d", num1, choice, num2);
    
		send(clientSocket, buffer, strlen(buffer), 0);
		bzero(buffer, sizeof(buffer));

		// bzero(buffer, sizeof(buffer));
		if(recv(clientSocket, buffer, 1024, 0) < 0){
			printf("Error in receiving data.\n");
		}else{
			printf("RESULT: \t%s\n", buffer);
		}
	}

	return 0;
}