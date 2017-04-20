//  David Mercado, Patrick True
//  CS485G Section 2
//  Project 5 - ssGet.c
//  21 April 201

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
	char *buffer[100] = {NULL};
	char *ServerRequest2[15] = {NULL};
	int ReturnedStatusNet[1];
    unsigned int caughtsize[1];
    int ReturnedStatus;
	unsigned int ValueReturnLength;
	unsigned long SecretKey;
	rio_t rio;
    
//------------------------------------------------------------------------------
	if(argc != 5)                                                               // Check for proper amount of command line arguments
	{
		// If not 5 command line arguments, output to stderr with usage and values that were input.
        fprintf(stderr, "usage: <MachineName> <TCPport> <SecurityKey> <VariableName>");
        exit(0);
	}
//------------------------------------------------------------------------------
	
	host = argv[1];                                                             // Assign the values into host and port
	port = atoi(argv[2]);

	// Build the first part of the server request with the security key and the request type
	SecretKey = atoi(argv[3]);                                                  //converts string secret key to unsigned long
    ServerRequest1[0] = htonl(SecretKey);                                       //converts secretkey to network order
    ServerRequest1[1] = 1;                                                      //type request ssGet
	
	memcpy(ServerRequest2, argv[4], 14);                                        // Build the second part of the server request with the variable name
	
	toserverfd = open_clientfd(host, port);                                     // Create file descriptor to the server

	Rio_readinitb(&rio, toserverfd);                                            // Ready for I/O
	 
	Rio_writen(toserverfd, ServerRequest1, sizeof(ServerRequest1));             // Send request to the server
	Rio_writen(toserverfd, ServerRequest2, sizeof(ServerRequest2));

    Rio_readnb(&rio,(void*)&ReturnedStatusNet, 4);                              //Sucess/ailure from the server
    ReturnedStatus = ntohl(ReturnedStatusNet[0]);                               //converts the returne success/fail value from net to host order

    if(ReturnedStatus == -1)                                                    //if fails output message
	{
		printf("Failure.\n");
	}
    else
    {
        Rio_readnb(&rio, (void *)&caughtsize, 4);                                   // Catch length of the value being returned
        ValueReturnLength = ntohl(caughtsize[0]);                                   // Network to Host order the returned length
        Rio_readnb(&rio, buffer, ValueReturnLength);                                // Catch the actual returned value requested
        printf("%s\n", buffer);
    }
    
    Close(toserverfd);                                                          //close connection to server
	exit(0);
}