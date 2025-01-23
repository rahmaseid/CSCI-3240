// Rahma Seid
// CSCI 3240
// Lab 9

#include "csapp.h"

int main(int argc, char *argv[])
{
    int clientfd;  //file descriptor to communicate with the server
    char *host, *port;
    size_t n; 
    
    char buffer[MAXLINE]; //MAXLINE = 8192 defined in csapp.h
    
    if (argc != 3)
    {
        fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);
	   exit(0);
    }
    
    host = argv[1];
    port = argv[2];

    clientfd = Open_clientfd(host, port);

    //getting a message from the user 
    printf("Please enter the message: ");
    
    //resetting the buffer
    bzero(buffer,MAXLINE);
    
    //getting the message from the user
    Fgets(buffer,MAXLINE,stdin);    

    //sending the message received from the user to the server
    n = write(clientfd,buffer,strlen(buffer));
    if(n<0){
        perror("Error!!");
        return(-1);
    }
        
    bzero(buffer,MAXLINE);
    //waiting for the message from the server.
    //the message will be stored in buffer variable.
    n = read(clientfd,buffer,MAXLINE); 
    if(n<0){
        perror("Error!!");
        return(-1);
    }
    
    printf("Message from Server:");
    //displaying the message in buffer on the console 
    Fputs(buffer,stdout);
    
    //getting a message from the user 
    printf("Please enter the second message: ");
    
    //resetting the buffer
    bzero(buffer,MAXLINE);
    
    //getting the message from the user
    Fgets(buffer,MAXLINE,stdin);    

    //sending the message received from the user to the server
    n = write(clientfd,buffer,strlen(buffer));
    if(n<0){
        perror("Error!!");
        return(-1);
    }
        
    bzero(buffer,MAXLINE);
    //waiting for the message from the server.
    //the message will be stored in buffer variable.
    n = read(clientfd,buffer,MAXLINE); 
    if(n<0){
        perror("Error!!");
        return(-1);
    }
    
    printf("Message from Server:");
    //displaying the message in buffer on the console 
    Fputs(buffer,stdout);
    
    Close(clientfd);
    return 0;
}