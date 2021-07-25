#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>


int main(int argc, char *argv[]){
    int Serversocket = socket(AF_INET,SOCK_STREAM,0);

    struct sockaddr_in serverAddr,clientAddr;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9800);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(Serversocket,(struct sockaddr *)&serverAddr,sizeof(serverAddr))<0){
        perror("Binding Failed");
        return -1;
    }

    if(listen(Serversocket,2)<0){
        perror("Listen failed");
        return -1;
    }

    printf("Server Started on port 9800...\n");

    int clientlen = sizeof(clientAddr);
    int clientsocket = accept(Serversocket,(struct sockaddr *)&clientAddr,&clientlen);
    if(clientsocket <0)
        perror("accept failed");
    
    int n = write(clientsocket,"Connected check\n",strlen("Connected check\n"));

    char buffer[255];

    FILE *fp;
    int ch = 0;
    fp = fopen("filerev.txt","w");  //change here
    int words;
    read(clientsocket,&words,sizeof(int));

    while(ch != words){
        read(clientsocket,buffer,255);
        //printf("rev::%s\n",buffer);
        fprintf(fp," %s",buffer);
        //fputs(buffer,fp);
        ch++;
    }

    printf("File Recieved\n");


    close(clientsocket);
    close(Serversocket);
    return 0;    
}