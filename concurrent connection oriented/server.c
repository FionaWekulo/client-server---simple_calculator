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

	int sockfd, ret;
	 struct sockaddr_in serverAddr;

	int newSocket;
	struct sockaddr_in newAddr;

	socklen_t addr_size;

	char buffer[1024];
	pid_t childpid;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		printf("Connection error!.\n");
		exit(1);
	}
	printf("Server Socket created successfully.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("Binding error.\n");
		exit(1);
	}
	printf("[+]Bind to port %d\n", 3000);

	if(listen(sockfd, 10) == 0){
		printf("Listening for clients....\n");
	}else{
		printf("Binding ERROR.\n");
	}


	while(1){
		newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0){
			exit(1);
		}
		printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

		if((childpid = fork()) == 0){
			close(sockfd);

			while(1){
				recv(newSocket, buffer, 1024, 0);
        // Parse input string
        char *int_string = strtok(buffer, " ");
        int num1 = atoi(int_string);
        int_string = strtok(NULL, " ");
        int op = atoi(int_string);
        int_string = strtok(NULL, " ");
        int num2 = atoi(int_string);

        bzero(buffer, sizeof(buffer));

				if(op == 5){
					printf("Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
					break;
				}
        // Perform calculation
        switch (op) {
          case 1:
              sprintf(buffer, "%d", num1 + num2);
              break;
          case 2:
              sprintf(buffer, "%ld", (long)num1 - num2);
              break;
          case 3:
              sprintf(buffer, "%d", num1 * num2);
              break;
          case 4:
              sprintf(buffer, "%f", (float)num1 / num2);
              break;
          default:
              sprintf(buffer, "Invalid operator");
        }

        printf("Sending result: %s\n", buffer);
        send(newSocket, buffer, strlen(buffer), 0);
				
			}
		}

	}

	close(newSocket);


	return 0;
}