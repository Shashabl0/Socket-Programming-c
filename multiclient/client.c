#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<semaphore.h>


void *Comm(void *sockID){
    int clientSocket = *((int *) sockID);

    while(1){

    }
}

int main(){
    int clientsocket = socket(AF_INET,SOCK_STREAM,0);

    struct sockaddr_in serverAddr;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9800);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(connect(clientsocket, (struct sockaddr *) &serverAddr,sizeof(serverAddr)) < 0)    
        return -1;

    printf("connection established...\n");

    //pthread_t thread;
    //pthread_create(&thread,NULL,)

    while(1){
        char buffer[1024];

        printf("Enter msg :: ");
        fgets(buffer,1024,stdin);

        int n = write(clientsocket,buffer,strlen(buffer));
        if(n<0)
            return -1;

        if(strncmp("quit",buffer,4)==0)
            break;
        
    }
    close(clientsocket);

    return 0;

}