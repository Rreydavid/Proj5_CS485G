//  David Mercado, Patrick True
//  CS485G Section 2
//  Project 5 - ssGet.c
//  21 April 2017

// Known Issues:
//					Line 44:  host to network long of 1 does not get seen by the server as 1 for "Get", sees "0" for set
//					Line 60:  warning:  passing argument 1 of Rio_readn makes integer from pointer without a cast (compile time)
//							  Rio_readin error: bad file descriptor (run time)



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
	char *buf[100];
	char *ServerRequest2[15] = {NULL};
	unsigned long SecretKey;
	rio_t rio;

	// Check for proper amount of command line arguments
	if(argc != 5)
	{
		// If not 5 command line arguments, output to stderr with usage and values that were input.
        fprintf(stderr, "usage: <MachineName> %s, <TCPport> %s, <SecurityKey> %s, <VariableName> %s \n ", argv[1], argv[2], argv[3], argv[4]);
        exit(0);
	}

	host = argv[1];
	port = atoi(argv[2]);

	// Build the first part of the server request with the security key and the request type
	SecretKey = atoi(argv[3]);														//converts string secret key to unsigned long
    ServerRequest1[0] = htonl(SecretKey);                                           //converts secretkey to network order
	ServerRequest1[1] = htonl(1);

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
	Rio_readn(&rio, buf, MAXLINE);
	Fputs(*buf, stdout);

	Close(toserverfd);
	exit(0);
}