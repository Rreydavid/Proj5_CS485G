//include all necessary headers
#include "csapp.h"
//begin main
int main(int argc, char **argv)
{
    int listenfd, connfd, port; //set up variables for your server connection
    socklen_t clientlen;
    struct sockaddr_in clientaddr;
    struct hostent *hp;
    char *haddrp;
//set up variables for the operations
    int fsize, fsizen;
    int  secretKey, clientSecret, secretn;
    int out, outcome;
    int list=0;
    char filelist[80][80];
    unsigned char *buff; 
    FILE* file;
    char filename[80];
    unsigned int clientype;
    rio_t confd;
    if (argc != 3) {//if incorrectly called on, exit
        fprintf(stderr, "usage: %s <port> <secretKey>\n", argv[0]);
        exit(0);
    }
    port = atoi(argv[1]);//assign listen port
    
    secretKey = atoi(argv[2]);//assign secretkey
    secretn=htonl(secretKey);//get the network order of secretkey
    listenfd = Open_listenfd(port);//listen
    while (1){//wait for connection
        printf("-----------------------------------\n");
        clientlen = sizeof(clientaddr);
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);

        /* Determine the domain name and IP address of the client */
        hp = Gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr,
                           sizeof(clientaddr.sin_addr.s_addr), AF_INET);

haddrp = inet_ntoa(clientaddr.sin_addr);
        Rio_readinitb(&confd,connfd);
//read in the secret key
        Rio_readnb(&confd,(void*)&clientSecret,4);
        int clients=ntohl(clientSecret);
        printf("Secret Key = %d\n", clients);
        //read in the type
        Rio_readnb(&confd,(void*)&clientype,4);
        clientype=ntohl(clientype);
        if (clientype==0)
        {
        printf("Type = get\n" );
        }
        else if (clientype==1)
        {
        printf("Type = put\n");
        }
        else if (clientype==2)
        {
        printf("Type = del\n");
        }
        else if (clientype==3)
        {
        printf("Type = list\n");
        }
       if (clientype==3)//if mclist, there is no file
        {
        printf("File Name = NONE\n");
}
        else{//otherwise, read in the file and print the name
        Rio_readnb(&confd,(void*)filename,80);
        printf("File Name = %s\n", filename);
        }
        if (secretn==clientSecret)//if the client secret matches the server secret
        {        if (clientype==0)
                {//mcget;retrieves contents of file and sends to client
			file=fopen(filename,"r");
			if (file==0)
			{
				perror(filename);
				outcome=-1;
			}
			else
			{
				outcome=0;
			}
			out=htonl(outcome);
		        Rio_writen(connfd,&out,4);
			if (file!=0)
			{
				fseek(file,0L,SEEK_END);
				fsize=ftell(file);
				fseek(file,0L,0);
				fgets(buff,fsize,file);
                        	fsizen=htonl(fsize);
				Rio_writen(connfd,&fsizen,4);
				Rio_writen(connfd,buff,fsize);
				fclose(file);
			}
                }
                else if (clientype==1)
                {//mcput; puts new file onto the server
                
                Rio_readnb(&confd,(void*)&fsize,4);
                fsize=ntohl(fsize);
		buff=(char*)malloc(fsize);
                Rio_readnb(&confd,(void*)buff,fsize);
                file=fopen(filename,"w");
		if (file!=0)
		{	
			if(fwrite(buff,1,fsize,file))//if it was successful
			{
				outcome=0;
				int i=0;
				while(i<list && strcmp(filename,filelist[i])!=0)//check to make sure file is not already in list
				{
					i++;
				}
				if (i==list)//add it to the list if it isn't.
				{
					strcpy(filelist[list],filename);
					list++;
				}
			}
			else if(!fwrite(buff,1,fsize,file))
			{
				outcome=-1;
			}
			out=htonl(outcome);
			Rio_writen(connfd,&out,4);
			fclose(file);
			
                }}
                else if (clientype==2)
                {//mcdel;deletes file from server
			int i=0;
			while(i<list && strcmp(filename,filelist[i])!=0)
			{
				i++;
			}
		
			if (strcmp(filename,filelist[i])==0)
			{
			unlink(filename);
			
				 if (i<list)
				{
					int j;
					for (j=i+1;j<list+1;j++)
					{
						strcpy(filelist[j-1],filelist[j]);
						i++;
					}
					list--;	
					outcome==0;
				 }
			}
			else
			{
				outcome=-1;
			}
			 out=htonl(outcome);	 
			 Rio_writen(connfd,&out,4);
                }
                else if (clientype==3)
                {//mclist; lists files currently stored on the server
			int count=0;
			int i;
			int size;
			for (i=0;i<list+1;i++)
			{
				size=strlen(filelist[i]);
				count=count+size;
			}
			buff=malloc(count);
			strcpy(buff,filelist[0]);	
			for (i=1;i<list;i++)
			{
				strcat(buff, "\n");
				strcat(buff,filelist[i]);
				count++;
			}
				count++;
			if (i==list)
			{
				outcome=0;
			}
			else
			{
				outcome=-1;
			}
				out=htonl(outcome);
				Rio_writen(connfd,&out,4);
				int countn=htonl(count);
				Rio_writen(connfd,&countn,4);
				Rio_writen(connfd,buff,count);
				
				
                }
	

        }
	else//if the secret key does not match, error
	{
		outcome=-1;
	}
	if (outcome==0)//if outcome is success, print so
	{
		printf("Operation Status= Success\n");
	}
	else
	{
		printf("Operation Status = Error\n");
	}
        Close(connfd);//clear the buffers
	if (clientype!=2)
	{
	memset((char **)buff,0,sizeof(buff));
	}
	if (clientype!=3)
	{
	memset((char **)filename,0,sizeof(filename));
   	 }}
    exit(0);
}

