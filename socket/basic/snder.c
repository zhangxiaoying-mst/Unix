#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include"proto.h"

int main(int argc,char*argv[])
{
	if(argc<2)
	{
		fprintf(stderr,"usage..\n");
		exit(1);
	}
	int sd;
	struct msg_st sbuf;
	struct sockaddr_in raddr;
	sd=socket(AF_INET,SOCK_DGRAM,0);

	if(sd<0)
	{
		perror("socket()");
		exit(1);
	}
//	bind();

	strcpy(sbuf.name,"Alan");
	sbuf.math=htonl(rand()%100);
	sbuf.chinese=htonl(rand()%100);
	
	raddr.sin_family=AF_INET;
	raddr.sin_port=htons(atoi(RCVPORT));
	inet_pton(AF_INET,argv[1],&raddr.sin_addr);

	if(sendto(sd,&sbuf,sizeof(sbuf),0,(void*)&raddr,sizeof(raddr))<0)
	{
		perror("sendto()");
		exit(1);

	}
	puts("OK!");


	exit(0);
}
