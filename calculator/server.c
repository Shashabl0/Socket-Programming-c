#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

void error(char *msg){
    perror(msg);
    exit(1);
}

int main(int argc,char *argv[]){
    
    if(argc<2){
        fprintf(stderr,"Port Number not provided\nProgram Terminated\n");
        exit(1);
    }
    int sockfd, newsockfd, portno, n;
    char buffer[255];

    struct sockaddr_in serv_addr,cli_addr;
    socklen_t clilen;    // get us internet access

    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd < 0){
        error("Error opening socket");
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));

    portno = atoi(argv[1]);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if(bind(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0)
        error("Binding Failed");
    listen(sockfd,5);
    clilen = sizeof(cli_addr);

    newsockfd = accept(sockfd,(struct sockaddr *)&cli_addr,&clilen);

    if(newsockfd <0){
        error("Error on accept");
	}

    int num1,num2,answer=0;
    char op;

    n =write(newsockfd,"Enter the Operation : ",strlen("Enter the Operation : "));
    if(n < 0)
        error("error writing to socket" );
    
    read(newsockfd,&num1,sizeof(int));
    read(newsockfd,&op,sizeof(int));
    read(newsockfd,&num2,sizeof(int));

    printf("we got values %d %c %d",num1,op,num2);
    switch(op){
        case '+':
            answer = num1 + num2;
            break;
        case '-':
            answer = num1 - num2;
            break;
        case '*':
            answer = num1 * num2;
            break;
        case '/':
            answer = num1/num2;
            break;
        default:
            answer = num1;
            break;
    }
    
    n = write(newsockfd,&answer,sizeof(int));
    if(n<0)
        error("error writing to sock");


	close(newsockfd);	
	close(sockfd);
    return 0;
}