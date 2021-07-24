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
int limit=2;
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
        buffer[0] = '\0';
        int r = read(clientsocket,buffer,1024);

        if(index == 0){
            int n = write(Client[1].sockid,buffer,1024);
        }
        else if(index == 1){
            int n = write(Client[0].sockid,buffer,1024);
        }
        if(strncmp("quit",buffer,4)==0)
            break;
            
        printf("client %d :: msg :: %s",index,buffer);
        buffer[0] = '\0';
        if(r<0)
            break;
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
    for(int i=0;i<limit;i++){    
        Client[i].sockid = accept(sockfd,(struct sockaddr *)&Client[i].clientAddr,&Client[i].len);
        Client[i].id = i;

        pthread_create(&thread[i],NULL,Comm,(void *) &Client[i]);
    }

    for(int i=0;i<2;i++){
        pthread_join(thread[i],NULL);
    }

	close(sockfd);
    return 0;
}
