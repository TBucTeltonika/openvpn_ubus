#include "telnet.h"


int telnet(char* ip_addr, int port, char *message, char *server_reply)
{
	int sock;
	struct sockaddr_in server;

	/*Create socket*/
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1)	{
		printf("Could not create socket");
	}
	puts("Socket created");

	server.sin_addr.s_addr = inet_addr(ip_addr);
	server.sin_family = AF_INET;
	server.sin_port = htons(port);

	/*Connect to remote server*/
	if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)	{	
		perror("connect failed. Error");
		exit(1);
	}
	char dummybuff[2056];

	/*Get the welcome msg. Before we send ours.*/
	if (recv(sock, dummybuff, 2056, 0) < 0)
		puts("Initial recv failed\n");

	
	if (send(sock, message, strlen(message), 0) < 0)	{
		puts("Send failed");
		exit(1);
	}
	/*Receive a reply from the server*/
	if (recv(sock, server_reply, 2056, 0) < 0)	{
		puts("recv failed");
		close(sock);
		exit(1);
	}
	else	{
		close(sock);
		return 0;
	}
}