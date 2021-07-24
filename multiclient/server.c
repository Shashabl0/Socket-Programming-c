#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<semaphore.h>

sem_t mutex;

struct client{
    int id;
    int sockid;
    struct sockaddr_in clientAddr;
    int len;
};
struct client Client[2];
pthread_t thread[2];

//checking if it supports outer function tho i m 100% sure it will
void printsuccess(){
    printf("how success tastes like ?\n");
}

void * Comm(void *ClientDet){
    struct client* clientDetail = (struct client*)ClientDet;
    int index = clientDetail->id;
    int clientsocket = clientDetail->sockid;

    printf("thread id %ld ",pthread_self());
    printf("Client %d is connected\n",index);
    while(1){
        char buffer[1024];

        int r = read(clientsocket,buffer,1024);
        if(strncmp("quit",buffer,4)==0)
            break;

        printf("Hey from client %d :: msg :: %s\n",index,buffer);
        buffer[r] == '\0';
        if(r<0)
            break;

        printsuccess();
    }
    return NULL;
}

int main(int argc,char *argv[]){
    
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    
    struct sockaddr_in serverAddr,clientAddr;
    bzero((char *) &serverAddr, sizeof(serverAddr));

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(9800);

    if(bind(sockfd,(struct sockaddr *)&serverAddr,sizeof(serverAddr))<0)
        return -1;

    if(listen(sockfd,2)<0)
        return -1;

    
    printf(" Server Started on port 9800..\n");

    //while(1){
        Client[0].sockid = accept(sockfd,(struct sockaddr *)&Client[0].clientAddr,&Client[0].len);
        Client[0].id = 1;

        pthread_create(&thread[0],NULL,Comm,(void *) &Client[0]);
        
        Client[1].sockid = accept(sockfd,(struct sockaddr *)&Client[1].clientAddr,&Client[1].len);
        Client[1].id = 2;

        pthread_create(&thread[1],NULL,Comm,(void *) &Client[1]);
    //}

    for(int i=0;i<2;i++){
        pthread_join(thread[i],NULL);
    }

	close(sockfd);
    return 0;
}
