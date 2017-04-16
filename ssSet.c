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
	char *host;                                                                 // Declare Variables
    int port;
    unsigned int ValueLength;
    unsigned long SecretKey;
    int FirstEightBytes[2];
    char *SecondFifthteenBytes[15] = {NULL};
    int ThirdForthBytes[1];
    char *Last100Bytes[100] = {NULL};
    int toserverfd;
	rio_t rio;
    
//------------------------------------------------------------------------------
	if(argc != 6)                                                               // Check for proper amount of command line arguments
	{
		// If not 6 command line arguments, output to stderr with usage and values that were input.
        fprintf(stderr, "Usage: ./ssSet host port secret variableName value\n");
        exit(0);
	}
    else if(sizeof(argv[5]) > 100)                                              //If Variable value exceeds 100 bytes
	{
		fprintf(stderr, "The value to be set must not exceed 100 bytes.\n");
		exit(0);
	}
    
//------------------------------------------------------------------------------
	host = argv[1];                                                             // set argv values into usable variables
    port = atoi(argv[2]);                                                       //converts port to int
    SecretKey = atoi(argv[3]);                                                  //converts string secret key to unsigned long
    ValueLength = sizeof(argv[5]);                                              //get size of value

    FirstEightBytes[0] = htonl(SecretKey);              //Bytes 0-3: A 4-byte unsigned integer containing SecretKey in network byte order
    FirstEightBytes[1]= 0;                             //Byte 4: A 1-byte unsigned integer containing the type of request: set (0)
    memcpy(SecondFifthteenBytes, argv[4], 14);              //Bytes 8-23: a null-terminated variable name, no longer than 15 characters.
    ThirdForthBytes[0] = htonl(ValueLength);        //Bytes 24-27: A 4-byte unsigned integer containing length of value in network order
    memcpy(Last100Bytes, argv[5], 99);                                          //Bytes 28 - Bytes....: The value itself.

//------------------------------------------------------------------------------
    toserverfd = Open_clientfd(host, port);                                     //Create file descriptor to the server, using wrapper
    Rio_readinitb(&rio, toserverfd);                                         //Associate a toserverfd with a read buffer and reset buffer
    
    Rio_writen(toserverfd, FirstEightBytes, sizeof(FirstEightBytes));           // Send to the server secret key and type request
    Rio_writen(toserverfd, SecondFifthteenBytes, sizeof(SecondFifthteenBytes)); // Send variable name
    Rio_writen(toserverfd, ThirdForthBytes, sizeof(ThirdForthBytes));           // send length of value for variable
    Rio_writen(toserverfd, Last100Bytes, sizeof(Last100Bytes));                 // Sends value of variable
	
//------------------------------------------------------------------------------
    Close(toserverfd);                                                          //close connection
	exit(0);
}