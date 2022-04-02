#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#define PORT 2137
#define CLIENTS_AMOUNT 5


int main(int argc, char *argv[]) {
	// Server and client file descriptors
	int listenfd = 0, connfd = 0;
	//structs holding info about connection on both sides
	struct sockaddr_in serv_addr,client_addr;
	socklen_t addr_size;
	char buffer[1024];

	// AF_INET - IPv4 
	// SOCK_STREAM - TCP/IP
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&serv_addr, '0', sizeof(serv_addr));

	// protocol family
	serv_addr.sin_family = AF_INET;
	// localhost
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	// set port 
	serv_addr.sin_port = htons(PORT); 

    // Bind server into specific port
	bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 
	listen(listenfd, CLIENTS_AMOUNT); 

	printf("Listening on port number: %i\n",PORT);
	while(1){
		connfd = accept(listenfd, (struct sockaddr *)&client_addr, &addr_size);
		printf("New connection\n");

		strcpy(buffer,"Hello from C");
		send(connfd,buffer,strlen(buffer),0);

		memset(buffer,'\0',sizeof(buffer));
		recv(connfd,buffer,1024,0);
		printf("Recived: %s\n",buffer);
	}
}

