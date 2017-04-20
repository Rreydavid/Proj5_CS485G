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
//~~~~~~~~~~~~~~~~~~~~PRINT MENU~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void PrintMenu(int SecretKey, char *Type, char *VariableName, char *Status)
{
    fprintf(stdout, "Secret Key = %d \n", SecretKey);                           //displays SecretKey provided by client
    fprintf(stdout, "Request type = %s \n", Type);                              //displays Request type provided by client
    fprintf(stdout, "Detail = %s \n", VariableName);                            //displays Details provided by client
    fprintf(stdout, "Operation Status = %s \n", Status);                        //displays sucess or failure
    fprintf(stdout, "-------------------------- \n");
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~







int main(int argc, const char * argv[])
{
    const int LISTEN_PORT = atoi(argv[1]);                                      //converts port to int (MIGHT CHANGE TO LONG)!!!!
    const unsigned long SECRET_KEY = atoi(argv[2]);                             //converts SecretKey to unsigned long
    const unsigned long LIMIT = 4294967296;                                     //(2^32-1) limit for SecretKey
    const int ROW = MAXLINE;
    const int CHAR_LENGTH = 100;
//------------------------------------------------------------------------------
    if(argc != 3)                                                               //checks for correct parameters
    {
        fprintf(stderr, "Incorrect number of parameters: <port> %s <SecretKey> %s \n ", argv[1], argv[2]);
        exit(0);
    }
    else if ((LISTEN_PORT <= 0) || (SECRET_KEY <= 0) || (SECRET_KEY >= LIMIT))  //checks for port and sceret key to be within limits
    {
        fprintf(stderr, "Incorrect Port or SecretKey Range!!: <port> %d <SecretKey> %ld \n ", LISTEN_PORT, SECRET_KEY);
        exit(0);
    }
    
//------------------------------------------------------------------------------
    char Record[ROW][CHAR_LENGTH];                                              //Initalized array to hold variables and values
    int RecordCount;
    int listenfd;                                                               //for listening socket
    int connfd;                                                                 //for accept
    struct sockaddr_in clientAddr;                                              //socket address structure for the internet
    rio_t rio;
    listenfd = Open_listenfd(LISTEN_PORT);                                      //open and return a listening socket on port (bind)
    socklen_t addrLength = sizeof(clientAddr);                                  //holds the length of the clients address
    
//------------------------------------------------------------------------------
    while(1)                                                                    //continously listening for clients
    {
        int SecretKeyNet[1];                                                     // Declare Variables
        int TypeRequest[1];
        char *VariableName[15] = {NULL};
        int ValueLengthNet[1];
        char *VariableValue[100] = {NULL};
        char *Type;
        char *Status;
        int pos=0;
        int compare=0;
        int VariableLen=0;
        int e;
        int ReturnedStatusNet;
        int ReturnedStatus[1];
        
        connfd = Accept(listenfd, (SA *)&clientAddr, &addrLength);              //accepts and creates a file descriptor for this connection
        
        Rio_readinitb(&rio,connfd);                                             //Creates internal buffer
        Rio_readnb(&rio,(void*)&SecretKeyNet, 4);                               //Reads Secret Key
        int SK = ntohl(SecretKeyNet[0]);                                        //Converts from Network order to host
        Rio_readnb(&rio, (void*)&TypeRequest, 4);                               //Reads request type (ssSet, ssGet...)
        
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        if(SK != SECRET_KEY)                                                    //if secret key doesnt match
        {
            printf("Secret Key = %d \n", SK);                                   //displays SecretKey provided by client
            Close(connfd);                                                      //closes connection to client
            continue;
        }
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        //~~~~~~~~~~~~ssSET~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        else if (TypeRequest[0] == 0)                                           //for ssSet client
        {
            Type = "set";
            Rio_readnb(&rio,VariableName, 15);                                  //reads in variable name

            Rio_readnb(&rio,(void*)&ValueLengthNet, 4);                         //Reads Value length
            int VL = ntohl(ValueLengthNet[0]);                                  //Converts Value length from network to host
            
            Rio_readnb(&rio,VariableValue, VL);                                 //reads in variable value
            VariableLen= strlen(VariableName);
            
            //===========================
            for(e = 0; e < RecordCount+1; e += 2)                               //Searching for Variable name
            {
                compare=strncmp(Record[e],VariableName,VariableLen);
                if(compare == 0)
                {
                    pos = e+1;
                    break;
                }
            }
            
            //===========================
            if(pos > 0)                                                         //inserting new value
            {
                memcpy(Record[pos], VariableValue, 99);
                Status = "Success";
            }
            
            //===========================
            else                                                                //inserting new variable and value
            {
                memcpy(Record[RecordCount], VariableName, strlen(VariableName)+1);
                memcpy(Record[RecordCount+1], VariableValue, VL+1);
                RecordCount += 2;
                Status = "Success";
            }
            
            ReturnedStatusNet= 0;
            ReturnedStatus[0]= htonl(ReturnedStatusNet);
            Rio_writen(connfd, (void*)&ReturnedStatus, 4);                      //Sends a 0 for success to client

            PrintMenu(SK, Type, VariableName, Status);                          //prints menu
            Close(connfd);                                                      //closes connection to client
        }
        
        //~~~~~~~~~~ssGET~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        else if (TypeRequest[0] == 1)                                           //for ssGet client
        {
            Type = "get";
            Rio_readnb(&rio,VariableName, 15);                                  //reads in variable name
            
            VariableLen= strlen(VariableName);                                  //sets the length of the variable name
            
            //===========================
            for(e = 0; e < RecordCount+1; e += 2)                               //Searching for Variable name
            {
                compare=strncmp(Record[e],VariableName,VariableLen);
                if(compare == 0)
                {
                    pos = e+1;
                    break;
                }
            }
            
            //===========================
            if(pos > 0)                                                         //inserting new variable and value or update
            {
                ReturnedStatusNet= 0;
                ReturnedStatus[0]= htonl(ReturnedStatusNet);
                Rio_writen(connfd, (void*)&ReturnedStatus, 4);                  //Sends a 0 for success to client
                
                unsigned int ValueLen = strlen(Record[pos]);                    //getting length of the value
                unsigned int ValueLenNet[1];
                ValueLenNet[0]=htonl(ValueLen);                                 //converting it to network order
                Rio_writen(connfd, ValueLenNet, 4);                             //sending length of value to client
                Rio_writen(connfd, Record[pos], ValueLen);                      //Sends value of variable
                
                Status = "Success";
            }

            //===========================
            else
            {
                ReturnedStatusNet= -1;
                ReturnedStatus[0]= htonl(ReturnedStatusNet);
                Rio_writen(connfd, (void*)&ReturnedStatus, 4);                   //Sends a -1 for success to client
                Status = "Failure";
            }
            
            PrintMenu(SK, Type, VariableName, Status);                          //prints menu
            Close(connfd);                                                      //closes connection to client
        }
        
        //~~~~~~~~~ssDIGEST~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        else if (TypeRequest[0] == 2)                                           //for ssDigest client
        {                                                                       //INCOMPLETE!!!!!!!!!!!!!!!!
            Type = "digest";
            Status = "Failure";

            PrintMenu(SK, Type, VariableName, Status);                          //prints menu
            Close(connfd);                                                      //closes connection to client
        }
        
        //~~~~~~~~ssRUN~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        else if (TypeRequest[0] == 3)                                           //for ssRun client
        {                                                                       //INCOMPLETE!!!!!!!!!!!!!!!!
            Type = "run";
            Status = "Failure";

            PrintMenu(SK, Type, VariableName, Status);                          //prints menu
            Close(connfd);                                                      //closes connection to client
        }
        
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        else                                                                    //if request type is incorrect
        {
            fprintf(stderr, "Incorrect Request Type. Connection will now close...\n");
            Close(connfd);                                                      //closes connection to client
        }
    }
    return 0;
}
