//  David Mercado, Patrick True
//  CS485G Section 2
//  Project 5 - ssSet.c
//  21 April 2017
//

#include <stdio.h>
#include <stdlib.h>
#include "csapp.h"

int main(int argc, char *argv[])
{
	// Declare Variables
	int toserverfd;
	int port;
	char *host;
	char *buf[MAXLINE];
	rio_t rio;

	// Check for proper amount of command line arguments
	if(argc != 6)
	{
		// If not 6 command line arguments, output to stderr with usage and values that were input.
        fprintf(stderr, "usage: <MachineName> %s, <TCPport> %s, <SecurityKey> %s, <VariableName> %, <Value> %s \n ", argv[1], argv[2], argv[3], argv[4], argv[5]);
        exit(0);
	}

	// Update the host and port variables with their values from ARGV
	host = argv[1];
	port = atoi(argv[2]);

	// Create file descriptor to the server
	toserverfd = open_clientfd(host, port);

	// Ready for I/O
	Rio_readinitb(&rio, toserverfd);

	// Get input
	printf("type: "); 
	fflush(stdout);

	while (Fgets(buf, MAXLINE, stdin) != NULL) 
	{
		// more input
		Rio_writen(toserverfd, buf, strlen(buf));
		// to server
		Rio_readlineb(&rio, buf, MAXLINE);
		// from server
		Fputs(buf, stdout);
		printf("type: "); 
		fflush(stdout);
	}

	close(toserverfd);
	exit(0);
}