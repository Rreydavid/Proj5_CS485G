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
//~~~~~~~~~~~~~~~~~~~~~~~~~~Insert~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/*int Insert(char *Record[MAXLINE], int *RecordCount, char *VariableName, char *VariableValue, int pos)
{
 
}*/
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
    const int ROW = MAXLINE;
    const int CHAR_LENGTH = 100;
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
    
    char Record[ROW][CHAR_LENGTH];                                              //Initalized array to hold variables and values
    int RecordCount;
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
        int SecretKeyNet[1];                                                     // Declare Variables
        int TypeRequest[1];
        char *VariableName[15] = {NULL};
        int ValueLengthNet[1];
        char *VariableValue[100] = {NULL};
        char *Type;
        char *Status;
        int pos=0;
        int Sitrep;
        int compare=0;
        int VariableLen=0;
        int e;

        
        connfd = Accept(listenfd, (SA *)&clientAddr, &addrLength);              //accepts and creates a file descriptor for this connection
        
        Rio_readinitb(&rio,connfd);                                             //Creates internal buffer
        Rio_readnb(&rio,(void*)&SecretKeyNet, 4);                               //Reads Secret Key
        int SK = ntohl(SecretKeyNet[0]);                                        //Converts from Network order to host

        Rio_readnb(&rio, (void*)&TypeRequest, 4);                               //Reads request type (ssSet, ssGet...)
        
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        if(SK != SECRET_KEY)
        {
            printf("Secret Key = %d \n", SK);                          //displays SecretKey provided by client
            Close(connfd);                                                      //closes connection to client
            continue;
        }
        
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        else if (TypeRequest[0] == 0)                                           //for ssSet client
        {
            Type = "set";
            Rio_readnb(&rio,VariableName, 15);                                  //reads in variable name

            Rio_readnb(&rio,(void*)&ValueLengthNet, 4);                         //Reads Value length
            int VL = ntohl(ValueLengthNet[0]);                                  //Converts Value length from network to host
            
            Rio_readnb(&rio,VariableValue, VL);                                 //reads in variable value
            VariableLen= strlen(VariableName);
            printf("My record Count prior to comparing: %d\n",RecordCount);
            printf("My Compare Value prior to comparing: %d\n",compare);

            for(e = 0; e < RecordCount+1; e += 2)                                   //Searching for Variable name
            {
                compare=strncmp(Record[e],VariableName,VariableLen);
                printf("Compare value: %d\n",compare);
                if(compare == 0)
                {
                    pos = e+1;
                    printf("Same value!!!\n");                                    //DELETE!!!!!!!!!!!
                    break;
                }
            }
            printf("Pos value: %d\n",pos);                                              //DELETE!!!!!!!!!!!

            if(pos > 0)                                                       //inserting new variable and value or update
            {
                printf("Im inside the pos>0\n");
                memcpy(Record[pos], VariableValue, 99);
                
                printf("Inside Insert else function: %s\n", Record[pos]);
                Sitrep=0;
            }
            else
            {
                printf("Im inside the else\n");

                memcpy(Record[RecordCount], VariableName, strlen(VariableName)+1);
                memcpy(Record[RecordCount+1], VariableValue, VL+1);
                RecordCount += 2;
                printf("RecordCount value : %d\n", RecordCount);
                Sitrep = 0;
            }
            
            if(Sitrep == -1)
            {
                Status = "Failure";
            }
            else
            {
                Status = "Success";
            }
            
            int r;
            for(r=0; r < RecordCount; r += 1)
            {
                printf("Record [%d]: %s\n",r, Record[r]);                       //DELETE!!!!!!!!!!!!!!!!

            }
            
            PrintMenu(SK, Type, VariableName, Status);                              //prints menu
        }
        
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        else if (TypeRequest[0] == 1)                                           //for ssGet client
        {
            Type = "get";
            Rio_readnb(&rio,VariableName, 15);                                  //reads in variable name
            
            VariableLen= strlen(VariableName);
            printf("My record Count prior to comparing: %d\n",RecordCount);
            printf("My Compare Value prior to comparing: %d\n",compare);
            
            for(e = 0; e < RecordCount+1; e += 2)                                   //Searching for Variable name
            {
                compare=strncmp(Record[e],VariableName,VariableLen);
                printf("Compare value: %d\n",compare);
                if(compare == 0)
                {
                    pos = e+1;
                    printf("Same value!!!\n");                                    //DELETE!!!!!!!!!!!
                    break;
                }
            }
            printf("Pos value: %d\n",pos);                                              //DELETE!!!!!!!!!!!
            
            if(pos > 0)                                                       //inserting new variable and value or update
            {
                printf("Im inside the pos>0\n");
                int ValueLen = strlen(Record[pos]);
                int ValueLenNet[1];
                ValueLenNet[0]=htonl(ValueLen);
                Rio_writen(connfd, ValueLenNet, 4);
                Rio_writen(connfd, Record[pos], ValueLen);                                  // Sends value of variable

                printf("Inside Insert else function: %s\n", Record[pos]);
                Sitrep=0;
            }
            else
            {
                int Number= -1;
                int ReturnFail[1];
                ReturnFail[0]= htonl(Number);
                Rio_writen(connfd, ReturnFail, 4);
                Sitrep = -1;
            }
            
            if(Sitrep == -1)
            {
                Status = "Failure";
            }
            else
            {
                Status = "Success";
            }
            
            int r;
            for(r=0; r < RecordCount; r += 1)
            {
                printf("Record [%d]: %s\n",r, Record[r]);                       //DELETE!!!!!!!!!!!!!!!!
                
            }
            
            PrintMenu(SK, Type, VariableName, Status);                              //prints menu
        }
        
        Close(connfd);                                                          //closes connection to client
    }
    return 0;
}
