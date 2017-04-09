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
    const unsigned long limit = 4294967296;
    
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
    
    
    
    
    
    
    
    fprintf(stdout, "Port number %d  SecretKey %ld \n", Port ,SecretKey);       //DELETE!!!!!!!!!!!!!!!!!!!!

    
    
    
    
    // BLAH BLAH BLAH, donkey wins.
    
    

    return 0;
}
