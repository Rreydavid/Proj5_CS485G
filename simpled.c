/*
 Course: CS485G
 Project: Project5
 Date: 04/10/2017
 Purpose: To make a simple server
 Author: Patrick True, David Mercado
 */

#include <stdio.h>
#include <stdlib.h>     //for atoi
#include "csapp.h"

int main(int argc, const char * argv[])
{
    const int Port = atoi(argv[1]);                                             //converts port to int (MIGHT CHANGE TO LONG)!!!!
    const unsigned long SecretKey = atoi(argv[2]);                              //converts SecretKey to unsigned long
    const unsigned long limit = 4294967296;                                     //(2^32-1) limit for SecretKey
    
    if(argc != 3)                                                         //checks for correct parameters
    {
        fprintf(stderr, "Incorrect number of parameters: <port> %s <SecretKey> %s \n ", argv[1], argv[2]);
        exit(0);
    }
    else if ((Port <= 0) || (SecretKey <= 0) || (SecretKey >= limit))
    {
        fprintf(stderr, "Incorrect Port or SecretKey Range!!: <port> %d <SecretKey> %ld \n ", Port, SecretKey);
        exit(0);
    }
    
    int listenfd;                                                               //for listening socket
    int connfd;
    struct sockaddr_in clientAddr;                                              //socket address structure for the internet
    struct hostent *clientHostEntry;                                            //used to represent an entry in the hosts database
    char *clientIP;                                                             //for clients IP address
    unsigned short clientPort;                                                  //for clients port number
    
    listenfd = open_listenfd(Port);                                             //open and return a listening socket on port (bind)
    socklen_t addrLength = sizeof(clientAddr);                                  //holds the length of the clients address

    while(1)                                                                    //continously listening for clients
    {
        connfd = Accept(listenfd, (SA *)&clientAddr, &addrLength);              //accepts and creates a file descriptor for this connection
        if(connfd < 1)
        {
            perror("accept error");
        }
        
        char * Request;
        char * Detail;
        char * Status;
        
        fprintf(stdout, "Secret Key = %ld \n", SecretKey);
        fprintf(stdout, "Request type = %s \n", Request);
        fprintf(stdout, "Detail = %s \n", Detail);
        fprintf(stdout, "Completion = %s \n", Status);
        fprintf(stdout, "-------------------------- \n");
        
        
    }
    
    
    
    
    
    fprintf(stdout, "Port number %d  SecretKey %ld \n", Port ,SecretKey);       //DELETE!!!!!!!!!!!!!!!!!!!!

    
    
    
    
    
    
    

    return 0;
}
