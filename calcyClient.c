/* --- Jaspreet Singh 
---  Client code for remote calculator on unix command line interface ---
*/


#include<stdio.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<stdlib.h>
#define MAX 80
#define PORT 43454
#define SA struct sockaddr
void error(char *msg)
{
  perror(msg);
  exit(1);
}
void calcy(int sockfd)
{
  char buff[MAX];
  int n;
  for(;;)
    {
      bzero(buff,sizeof(buff));
      printf("\nEnter the Expression: ");
      n=0;
      while((buff[n++]=getchar())!='\n');
      write(sockfd,buff,sizeof(buff)); //client is going to write the expression on socket
      bzero(buff,sizeof(buff));//clearing the buffer
      read(sockfd,buff,sizeof(buff));//reading the result fom socket
      printf("Result: %s",buff);
      if((strncmp(buff,"exit",4))==0)
	{
	  printf("Client Exit...\n");
	  break;
	}
    }
}
int main(int argc,char *argv[])
{
  int sockfd,connfd,portno;
  struct sockaddr_in servaddr,cli;
  sockfd=socket(AF_INET,SOCK_STREAM,0); //creating socket
  portno=atoi(argv[1]);
  if(argc<3){
    fprintf(stderr,"Error please provide both port number and ip address");
    exit(1);
  }
  if(sockfd==-1)
    {
      printf("socket creation failed...\n");
      exit(0);
    }
  else
    printf("Socket successfully created..\n");
  bzero(&servaddr,sizeof(servaddr));
  servaddr.sin_family=AF_INET;//socket belongs to internet
  servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
  servaddr.sin_port=htons(portno);//converting port number to network address
  if(connect(sockfd,(SA *)&servaddr,sizeof(servaddr))!=0)//connecting to the server
    {
      printf("connection with the server failed...\n");
      exit(0);
    }
  else
  printf("connected to the server..\n");
  calcy(sockfd);//calling calcy function
  close(sockfd);//closing socket
}
