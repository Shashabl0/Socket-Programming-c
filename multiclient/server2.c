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
int CLIENT_LIMIT=3;
int active = 0;
int disconnt = 0;
int flag = 0;

struct client{
    int id;
    int sockid;
    struct sockaddr_in clientAddr;
    int len;
    int connect;
};
struct client Client[2];
pthread_t thread[2];

void *rev(void *clientDetails){
    struct client* clientDetail = (struct client*)clientDetails;
    int index = clientDetail->id;
    int clientsocket = clientDetail->sockid;
    char buffer[1024];
    printf(" connet id ::%d::",clientDetail->connect);
    printf(" data buff ::%s::",buffer);
    while(1){
        read(clientDetail->connect,buffer,1024);
        if(strlen(buffer)!=0){
            write(clientsocket,buffer,1024);
            printf("buff 6 client %d :: msg :: %s\n",index,buffer);
        }

        if(strncmp("bye",buffer,3)==0)
            break;
        buffer[0]='\0';
    }
        //    }
}

void * Comm(void *ClientDet){
    struct client* clientDetail = (struct client*)ClientDet;
    int index = clientDetail->id;
    int clientsocket = clientDetail->sockid;

    printf("thread id %ld ",pthread_self());
    printf("Client %d is connected\n",index);
    
    char options[]="\n   CHAT SERVERv1.1\nyou have options:\n LIST: lists active clients\n SEND: working\n QUIT: to close connection\n\n";
    
    int w = write(clientsocket,options,1024);
    if(w < 0){
        perror("options Write failed");
    }
    int t = 1;  //counter for default case
    while(1){
        
        char databuffer[100];
        int r = read(clientsocket,databuffer,100);
        if(r<0)
            perror("option read failed");
        char buffer[1024];
        
        if(strncmp("Y",databuffer,1)==0){
            pthread_t t1; 
            pthread_create(&t1,NULL,rev,(void *)&clientDetail);
            pthread_join(t1,NULL);
        }

        if(strncmp("LIST",databuffer,4)==0){
            //********************************LIST FEATURE***********************************
            w = write(clientsocket,"Listing Client Avaiable\n",1024);
            for(int j=0;j<active;j++){
                if(Client[j].sockid == -1)
                    continue;
                if( Client[j].id != index ){        //client Id here
                    snprintf (buffer, sizeof(buffer), "Client %d\n",Client[j].id);
                    w = write(clientsocket,buffer,1024);
                    buffer[0]='\0';
                }
            }
            if(active-1 == disconnt){
                w = write(clientsocket,"No other Client available\n",1024);
            }
            //********************************ENDS HERE***********************************************
        }
        else if(strncmp("SEND",databuffer,4)==0){
            //*********************************SEND FEATURE******************************************
            // insert msg function
            char dbuffer[255];
            int i=0;
            w = write(clientsocket,"(to)\n",1024);
            if(w < 0)
                perror("write1 failed");
            dbuffer[0]='\0';
            while(1){
                r = read(clientsocket,dbuffer,1024);
                if(r < 0)
                    perror("read failed 1");
                printf(" BUFF1 ::%s: strlen : %ld\n",dbuffer,strlen(dbuffer));
                if(strlen(dbuffer) !=0)
                    break;
            }
            for(i=0;i<active;i++){
                if(Client[i].sockid == -1)
                    continue;
                if(Client[i].id == atoi(dbuffer) && index != atoi(dbuffer)){
                    printf("its a match !!\n");
                    break;
                }
            }
            if(Client[i].id == atoi(dbuffer) && i!=active){
                    dbuffer[0]='\0';
                    struct client cli = Client[i];
                    
                    w = write(cli.sockid,"someone want to chat accept?(Y/N)\n",strlen("someone want to chat accept?(Y/N)\n"));
                    while(1){
                        r = read(cli.sockid,dbuffer,1024);
                        if(r < 0)
                            perror("read failed 1");
                        if(strlen(dbuffer) !=0)
                            break;
                    }

                    printf(" buff3::%s::strlen::%ld::\n",dbuffer,strlen(dbuffer));

                    if(strncmp("Y",dbuffer,1)==0){
                        Client[i].connect = clientsocket;
                        while(1){
                            dbuffer[0] = '\0';
                            while(1){
                                r = read(clientsocket,dbuffer,1024);
                                if(r < 0)
                                    perror("read failed 1");
                                printf(" BUFF4 ::%s: strlen : %ld\n",dbuffer,strlen(dbuffer));
                                if(strlen(dbuffer) !=0)
                                    break;
                            }
                            int n = write(cli.sockid,dbuffer,1024);
                                
                            printf("buff 5 client %d :: msg :: %s\n",index,dbuffer);

                            if(strncmp("bye",dbuffer,3)==0){
                                printf("connection closed\n");
                                cli.connect = 0;
                                break;
                            }
                        }
                    }
                    else if(strncmp("N",dbuffer,1)==0){
                        w = write(clientsocket,"Sorry he denied for connect..\n",strlen("Sorry he denied for connect..\n"));
                    }
            }
            else{
                perror("no Client of that id present !!");
            }
            
            //here
            //*********************************ENDS HERE**********************************************
        }
        else if(strncmp("SELF",databuffer,4)==0){
            //*********************************SELF FEATURE******************************************
            snprintf (buffer, sizeof(buffer), " ID - %d\n",index);
            w = write(clientsocket,buffer,1024);
            //*********************************ENDS HERE**********************************************
        }
        else if(strncmp("QUIT",databuffer,4)==0){
            //********************************QUIT FEATURE*******************************************
            w = write(clientsocket,"QUIT",1024);
            clientDetail->sockid = -1;
            disconnt++;
            printf("\nAvailable clients %d\n",active-disconnt);
            break;
            //*********************************ENDS HERE**********************************************
        }
        buffer[0]='\0';
        //*********DEFAULT************************
        /*else if(databuffer != "" && t > 0){
            printf("Databuffer :: %s\n",databuffer);
            //strcat(buffer,"Be in you CLIENT_LIMIT right now :P its not yet complete\n");
            w = write(clientsocket,"Be in you CLIENT_LIMIT right now :P its not yet complete\n",51);
            buffer[0]='\0';
            t--;    //counter for this program
        }
        */
        //********ENDS***************************
        if(w < 0){
            perror("options Write failed");
        }
        databuffer[0]='\0';
    }
    return NULL;
}

int main(int argc,char *argv[]){
    
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    
    struct sockaddr_in serverAddr;
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
    for(int i=0;i<CLIENT_LIMIT && flag==0;i++){    
        Client[i].sockid = accept(sockfd,(struct sockaddr *)&Client[i].clientAddr,&Client[i].len);

        pthread_create(&thread[i],NULL,Comm,(void *) &Client[i]);
        Client[i].id = thread[i]%1000;
        Client[i].connect = 0;
        active++;
        if(active - disconnt == 0){
            break;
        }
    }

    for(int i=0;i<active;i++){
        pthread_join(thread[i],NULL);
    }

	close(sockfd);
    return 0;
}
