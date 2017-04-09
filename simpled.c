/*
 Course: CS485G
 Project: Project5
 Date: 04/10/2017
 Purpose: To make a simple server
 Author: Patrick True, David Mercado
 */

#include <stdio.h>
#include <stdlib.h>     //for atoi
#include "csapp.h"      //for sockets and what not

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~ssSet~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int simpleSet(char *MachineName, int TCPport, int SecretKey, char *variableName, char *value, int dataLength)
{
    return 1;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~ssGet~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int simpleGet(char *MachineName, int TCPport, int SecretKey, char *variableName, char *value, int *resultLength)
{
    return 1;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int main(int argc, const char * argv[])
{
    const int LISTEN_PORT = atoi(argv[1]);                                      //converts port to int (MIGHT CHANGE TO LONG)!!!!
    const unsigned long SECRET_KEY = atoi(argv[2]);                             //converts SecretKey to unsigned long
    const unsigned long LIMIT = 4294967296;                                     //(2^32-1) limit for SecretKey
    const int ROW = 50;                                                         
    const int COLUMN = 2;

    if(argc != 3)                                                               //checks for correct parameters
    {
        fprintf(stderr, "Incorrect number of parameters: <port> %s <SecretKey> %s \n ", argv[1], argv[2]);
        exit(0);
    }
    else if ((LISTEN_PORT <= 0) || (SECRET_KEY <= 0) || (SECRET_KEY >= LIMIT))
    {
        fprintf(stderr, "Incorrect Port or SecretKey Range!!: <port> %d <SecretKey> %ld \n ", LISTEN_PORT, SECRET_KEY);
        exit(0);
    }
    
    char *Record[ROW][COLUMN];                                                  //Initalized 2 deminsional array to hold variables
    int listenfd;                                                               //for listening socket
    int connfd;                                                                 //for accept
    struct sockaddr_in clientAddr;                                              //socket address structure for the internet
    struct hostent *clientHostEntry;                                            //used to represent an entry in the hosts database
    char *clientIP;                                                             //for clients IP address
    unsigned short clientPort;                                                  //for clients port number
    
    listenfd = open_listenfd(LISTEN_PORT);                                      //open and return a listening socket on port (bind)
    socklen_t addrLength = sizeof(clientAddr);                                  //holds the length of the clients address

    while(1)                                                                    //continously listening for clients
    {
        connfd = Accept(listenfd, (SA *)&clientAddr, &addrLength);              //accepts and creates a file descriptor for this connection
        if(connfd < 1)
        {
            perror("accept error");
        }
        
        
        
        
        
        
        
        
        
        
        unsigned long Password;
        char * Request;                                                         //store Request type
        char * Detail;                                                          //store Details
        char * Status;                                                          //store if its successful or not
        
        fprintf(stdout, "Secret Key = %ld \n", Password);                       //displays SecretKey provided by client
        fprintf(stdout, "Request type = %s \n", Request);                       //displays Request type provided by client
        fprintf(stdout, "Detail = %s \n", Detail);                              //displays Details provided by client
        fprintf(stdout, "Completion = %s \n", Status);
        fprintf(stdout, "-------------------------- \n");
        Close(connfd);                                                          //closes connection to client
        
        fprintf(stdout, "Port number %d  SecretKey %ld \n", LISTEN_PORT ,SECRET_KEY);       //DELETE!!!!!!!!!!!!!!!!!!!!

    }
    return 0;
}
