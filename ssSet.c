//  David Mercado, Patrick True
//  CS485G Section 2
//  Project 5 - ssSet.c
//  21 April 2017
//

#include <stdio.h>
#include <stdlib.h>
#include "csapp.h"
#include <arpa/inet.h>      //for network ordering

int main(int argc, char *argv[])
{
    const int MAX_REQUEST_BUFF = 128;                                           // Declare Variables
	char *host;
    int port;
    unsigned long SecretKey;
    unsigned long SecretKeyNet;
    char *SetRequest[MAX_REQUEST_BUFF] = {NULL};
    char *VariableName[MAXLINE];                                                //DELETE!!!!!!!!!!
	char *VariableValue[MAXLINE];                                               //DELETE!!!!!!!!!!
    int toserverfd;
	rio_t rio;
    
//------------------------------------------------------------------------------
	if(argc != 6)                                                               // Check for proper amount of command line arguments
	{
		// If not 6 command line arguments, output to stderr with usage and values that were input.
        fprintf(stderr, "usage: <MachineName> %s, <TCPport> %s, <SecurityKey> %s, <VariableName> %s, <Value> %s \n ", argv[1], argv[2], argv[3], argv[4], argv[5]);
        exit(0);
	}
    else if(sizeof(argv[5]) > 100)                                              //If Variable value exceeds 100 bytes
	{
		fprintf(stderr, "The value to be set must not exceed 100 bytes.");
		exit(0);
	}
    
//------------------------------------------------------------------------------
	host = argv[1];                                                             // set argv values into usable variables
	port = atoi(argv[2]);
    SecretKey = atoi(argv[3]);                                                  //converts string secret key to unsigned long
    SecretKeyNet = htonl(SecretKey);                                            //converts secretkey to network order
    
    
    *VariableName = argv[4];
	*VariableValue = argv[5];
    
    
    SetRequest[0] = argv[0];                                //Bytes 0-3: A 4-byte unsigned integer containing SecretKey in network byte order
    SetRequest[4] = 0;                                      //Byte 4: A 1-byte unsigned integer containing the type of request: set (0)
    SetRequest[8] = argv[4];                                //Bytes 8-23: a null-terminated variable name, no longer than 15 characters.
    SetRequest[24] = argv[0];                        //Bytes 24-27: A 4-byte unsigned integer (in network order) giving the length of the value
    SetRequest[28] = argv[5];           //Bytes 28 ..: The value itself. The client need not send any more than the number of bytes required.
    
    
    toserverfd = Open_clientfd(host, port);                                     //Create file descriptor to the server, using wrapper
	// Ready for I/O
    Rio_readinitb(&rio, toserverfd);                                            //Associate a toserverfd with a read buffer and reset buffer

	// Send to the server
    Rio_writen(toserverfd, *argv[3], strlen(argv[3]));

	Rio_writen(toserverfd, *VariableName, strlen(*VariableName));
	Rio_writen(toserverfd, *VariableValue, strlen(*VariableValue));

	// From the server
    Rio_readnb(&rio, *VariableName, 1);                                //reads buffer from sever
	Fputs(*VariableName, stdout);

	Close(toserverfd);
	exit(0);
}