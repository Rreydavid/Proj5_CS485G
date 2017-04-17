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
//~~~~~~~~~~~~~~~~~~~~~~~~~~~Search~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int Search(&Record[MAXLINE][], char *VariableName)
{
    int pos;
    
    for(pos = 0; pos < MAXLINE; pos += 2)
    {
        if(Record[pos] == NULL)
        {
            printf("I worked!!!\n");                                              //DELETE!!!!!!!!!!!
            return -1;
        }
        else if(Record[pos] == VariableName)
        {
            return pos+1;
        }
    }
    
    printf("I DIDNT work\n");                                                 //DELETE!!!!!!!!!!!
    return -1;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~Insert~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int Insert(char *Record[MAXLINE], int *RecordCount, char *VariableName, char *VariableValue, int pos)
{
    if(pos == -1)
    {
        Record[*RecordCount] = VariableName;
        Record[*RecordCount+1] = VariableValue;
        *RecordCount += 2;
        
        fprintf("Inside Insert -1 function: %c : %c", Record[*RecordCount], Record[*RecordCount+1]);
        return 0;
    }
    else
    {
        Record[pos] = VariableValue;
        
        fprintf("Inside Insert else function: %s",Record[pos]);
        return 0;
    }
    
    return -1;
}
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
    
//------------------------------------------------------------------------------
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
    
//------------------------------------------------------------------------------
    int SecretKeyNet[1];                                                        // Declare Variables
    int TypeRequest[1];
    char *VariableName[15] = {NULL};
    int ValueLengthNet[1];
    char *VariableValue[100] = {NULL};
    char *Type;
    char *Status;
    char Record[MAXLINE][100];                                              //Initalized array to hold variables and values
    int RecordCount;
    int pos;
    int Sitrep;
    int listenfd;                                                               //for listening socket
    int connfd;                                                                 //for accept
    struct sockaddr_in clientAddr;                                              //socket address structure for the internet
    //    struct hostent *clientHostEntry;                                            //used to represent an entry in the hosts database
    rio_t rio;
    
    listenfd = Open_listenfd(LISTEN_PORT);                                      //open and return a listening socket on port (bind)
    socklen_t addrLength = sizeof(clientAddr);                                  //holds the length of the clients address
    
//------------------------------------------------------------------------------
    while(1)                                                                    //continously listening for clients
    {
        connfd = Accept(listenfd, (SA *)&clientAddr, &addrLength);              //accepts and creates a file descriptor for this connection
        
        Rio_readinitb(&rio,connfd);                                             //Creates internal buffer
        Rio_readnb(&rio,(void*)&SecretKeyNet, 4);                               //Reads Secret Key
        int SK = ntohl(SecretKeyNet[0]);                                        //Converts from Network order to host

        Rio_readnb(&rio, (void*)&TypeRequest, 4);                               //Reads request type (ssSet, ssGet...)
        
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        if(SK != SECRET_KEY)
        {
            fprintf(stdout, "Secret Key = %d \n", SK);                          //displays SecretKey provided by client
            Close(connfd);                                                      //closes connection to client
            continue;
        }
        
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        else if (TypeRequest[0] == 0)                                           //for ssSet client
        {
            Type = "set";
            Rio_readnb(&rio,VariableName, 15);                                  //reads in variable name

            Rio_readnb(&rio,(void*)&ValueLengthNet, 4);                         //Reads Variable length
            int VL = ntohl(ValueLengthNet[0]);                                  //Converts variable length from network to host
            
            Rio_readnb(&rio,VariableValue, VL);                                 //reads in variable value

            pos = Search(Record, VariableName);
            printf("Pos value %d\n", pos);                                //delete!!!!!!!!!!!!!!!

            
   
            Sitrep = Insert(&Record[MAXLINE], &RecordCount, VariableName, VariableValue, pos);  //add to array
            printf("Sitrep value %d\n", Sitrep);                                //delete!!!!!!!!!!!!!!!
            
            if(Sitrep == -1)
            {
                Status = "Failure";
            }
            else
            {
                Status = "Success";
            }
            
            printf("array %s\n", Record[0]);                       //DELETE!!!!!!!!!!!!!!!!
            printf("array %s\n", Record[1]);                       //DELETE!!!!!!!!!!!!!!!!
            printf("array %s\n", Record);                       //DELETE!!!!!!!!!!!!!!!!

            PrintMenu(SK, Type, VariableName, Status);                          //prints menu
        }
        
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        else if (TypeRequest[0] == 1)                                           //for ssGet client
        {
            printf("Not ready yet");

        }
        
        Close(connfd);                                                          //closes connection to client
    }
    return 0;
}
