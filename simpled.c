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
    //    struct hostent *clientHostEntry;                                      //used to represent an entry in the hosts database
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
            printf("My record Count prior to comparing: %d\n",RecordCount);     //DELETE!!!!!!!!
            printf("My Compare Value prior to comparing: %d\n",compare);        //DELETE!!!!!!!!
            
            //===========================
            for(e = 0; e < RecordCount+1; e += 2)                               //Searching for Variable name
            {
                compare=strncmp(Record[e],VariableName,VariableLen);
                printf("Compare value: %d\n",compare);                          //DELETE!!!!!!!!
                if(compare == 0)
                {
                    pos = e+1;
                    printf("Same value!!!\n");                                  //DELETE!!!!!!!!!!!
                    break;
                }
            }
            printf("Pos value: %d\n",pos);                                      //DELETE!!!!!!!!!!!
            //===========================
            if(pos > 0)                                                         //inserting new variable and value or update
            {
                printf("Im inside the pos>0\n");                                //DELETE!!!!!!!!
                memcpy(Record[pos], VariableValue, 99);
                printf("Inside Insert else function: %s\n", Record[pos]);       //DELETE!!!!!!!!
                Status = "Success";
            }
            //===========================
            else
            {
                printf("Im inside the else\n");                                 //DELETE!!!!!!!!
                memcpy(Record[RecordCount], VariableName, strlen(VariableName)+1);
                memcpy(Record[RecordCount+1], VariableValue, VL+1);
                RecordCount += 2;
                printf("RecordCount value : %d\n", RecordCount);                //DELETE!!!!!!!!
                Status = "Success";
            }
   
            char response = 0;                                              //MIGHT NEED TO SEND 3 ADDIOTIONAL BYTES OF PADDING
            Rio_writen(connfd, response, 4);                                //Sends a 0 for success to client
            printf("Returned Response is: %c\n",response);                   //DELETE!!!!!!!!

            int r;
            for(r=0; r < RecordCount; r += 1)
            {
                printf("Record [%d]: %s\n",r, Record[r]);                       //DELETE!!!!!!!!!!!!!!!!
            }
            
            PrintMenu(SK, Type, VariableName, Status);                          //prints menu

            Close(connfd);                                                      //closes connection to client

        }
        
        //~~~~~~~~~~ssGET~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        else if (TypeRequest[0] == 1)                                           //for ssGet client
        {
            Type = "get";
            Rio_readnb(&rio,VariableName, 15);                                  //reads in variable name
            printf("My VariableName is: %s\n",VariableName);                    //DELETE!!!!!!!!
            
            VariableLen= strlen(VariableName);                                  //sets the length of the variable name
            printf("My Variabel length is: %d\n",VariableLen);     //DELETE!!!!!!!!
            printf("My record Count prior to comparing: %d\n",RecordCount);     //DELETE!!!!!!!!
            printf("My Compare Value prior to comparing: %d\n",compare);        //DELETE!!!!!!!!
            
            //===========================
            for(e = 0; e < RecordCount+1; e += 2)                               //Searching for Variable name
            {
                compare=strncmp(Record[e],VariableName,VariableLen);
                printf("Compare value: %d\n",compare);                          //DELETE!!!!!!!!
                if(compare == 0)
                {
                    pos = e+1;
                    printf("Same value!!!\n");                                  //DELETE!!!!!!!!!!!
                    break;
                }
            }
            printf("Pos value: %d\n",pos);                                      //DELETE!!!!!!!!!!!
            
            //===========================
            if(pos > 0)                                                         //inserting new variable and value or update
            {
                char response = 0;                                              //MIGHT NEED TO SEND 3 ADDIOTIONAL BYTES OF PADDING
                Rio_writen(connfd, response, 4);                                //Sends a 0 for success to client
                printf("Returned Response is: %c\n",response);                   //DELETE!!!!!!!!
                
                unsigned int ValueLen = strlen(Record[pos]);                    //getting length of the value
                unsigned int ValueLenNet[1];
                ValueLenNet[0]=htonl(ValueLen);                                 //converting it to network order
                Rio_writen(connfd, ValueLenNet, 4);                             //sending length of value to client
                Rio_writen(connfd, Record[pos], ValueLen);                      //Sends value of variable
                printf("JUST SENT: %s\n", Record[pos]);                         //DELETE!!!!!!!!
                
                Status = "Success";
            }

            //===========================
            else
            {
                char response = -1;
                Rio_writen(connfd, response, 4);                                //returns a -1 for failure
                Status = "Failure";
            }
            
            int r;                                                              //DELETE!!!!!!!!
            for(r=0; r < RecordCount; r += 1)                                   //DELETE!!!!!!!!
            {printf("Record [%d]: %s\n",r, Record[r]); }                      //DELETE!!!!!!!!!!!!!!!!
            
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
        else
        {
            fprintf(stderr, "Incorrect Request Type. Connection will now close...\n");
            Close(connfd);                                                      //closes connection to client
        }
    }
    return 0;
}
