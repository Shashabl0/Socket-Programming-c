#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include<ctype.h>

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int Clientsocket = socket(AF_INET,SOCK_STREAM,0);

    struct sockaddr_in clientAddr;

    clientAddr.sin_family = AF_INET;
    clientAddr.sin_port = htons(9800);
    clientAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if(connect(Clientsocket,(struct sockaddr *)&clientAddr,sizeof(clientAddr))<0){
        perror("Connect failed");
        return -1;
    }

    printf("Connection established...\n");

    char buffer[255];

    int n = read(Clientsocket,buffer,255);
    printf("msg received :: %s",buffer);

    buffer[n]='\0'; //clear buffer

    FILE *f;
    
    int words=0;
    char c = ' ';
    f = fopen("file.txt","r");  // file open
    while((c) != EOF){
        fscanf(f,"%s",buffer);
        printf("read0::%s\n",buffer);
        if(isspace(c)||c=='\t')
            words++;
        c=getc(f);
    }
    write(Clientsocket,&words,sizeof(int));

    buffer[0]='\0';
    //rewind(f);  //make pointer to start
    fseek(f, 0, SEEK_SET);
    char ch;
    
    /*
    while(ch!=EOF){
        fgets(buffer,50,f);
        printf("read1::%s",buffer);
        write(Clientsocket,buffer,strlen(buffer));
        ch = fgetc(f);
        printf("\nchar c::%c",c);
        buffer[0]='\0';
    }
    */
    while(ch!=EOF){
        fscanf(f,"%s",buffer);
        if(ch == '\n')
            printf("\nnewline found\n");
        printf("\nread1::%s",buffer);
        write(Clientsocket,buffer,255);
        ch = fgetc(f);
    }

    printf("\nFile Send Successfull");

    close(Clientsocket);
    return 0;
}