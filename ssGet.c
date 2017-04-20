//  David Mercado, Patrick True
//  CS485G Section 2
//  Project 5
//  21 April 201
//  ssGet Client

#include <stdio.h>
#include <stdlib.h>
#include "csapp.h"
#include <arpa/inet.h>      //for network ordering

int main(int argc, char *argv[])
{
	// Declare Variables
	int toserverfd;
	int ServerRequest1[2];
	char *host;
	int port;
	int ReturnedValue;
	char *buffer[100] = {NULL};
	char *ServerRequest2[15] = {NULL};
	int caughtsize[1];
	int ValueReturnLength;
	unsigned long SecretKey;
	rio_t rio;

	// Check for proper amount of command line arguments
	if(argc != 5)
	{
		// If not 5 command line arguments, output to stderr with usage and values that were input.
        fprintf(stderr, "usage: <MachineName> %s, <TCPport> %s, <SecurityKey> %s, <VariableName> %s \n ", argv[1], argv[2], argv[3], argv[4]);
        exit(0);
	}

	// Assign the values into host and port
	host = argv[1];
	port = atoi(argv[2]);

	// Build the first part of the server request with the security key and the request type
	SecretKey = atoi(argv[3]);														//converts string secret key to unsigned long
    ServerRequest1[0] = htonl(SecretKey);                                           //converts secretkey to network order
	ServerRequest1[1] = 1;

	// Build the second part of the server request with the variable name
	memcpy(ServerRequest2, argv[4], 14);

	// Create file descriptor to the server
	toserverfd = open_clientfd(host, port);

	// Ready for I/O
	Rio_readinitb(&rio, toserverfd);
	 
	// Send request to the server
	Rio_writen(toserverfd, ServerRequest1, sizeof(ServerRequest1));
	Rio_writen(toserverfd, ServerRequest2, sizeof(ServerRequest2));

	// from the server
	// Catch the success or failure byte, 3 trash bytes
	rio_readnb(&rio, (void*)&ReturnedValue, 4);

	if(ReturnedValue == -1)
	{
		printf("Failure.");
	}

	// Catch length of the value being returned
	Rio_readnb(&rio, (void *)&caughtsize, 4);
	// Network to Host order the returned length 
	ValueReturnLength = ntohl(caughtsize[0]);

	// Catch the actual returned value requested
	Rio_readnb(&rio, buffer, ValueReturnLength);
	printf("%s\n", buffer);	
	
	Close(toserverfd);
	exit(0);
}