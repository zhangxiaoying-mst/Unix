#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include"proto.h"
#define IPSTRSIZE 40

int main()
{
	int sd;
	struct sockaddr_in laddr,raddr;
	struct msg_st *rbufp;
	socklen_t raddr_len;
	char ipstr[IPSTRSIZE];
	int size;

	size=sizeof(struct msg_st)+NAMEMAX-1;
	rbufp=malloc(size);
	if(rbufp==NULL)
	{
		perror("malloc()");
		exit(1);
	}

	sd=socket(AF_INET,SOCK_DGRAM,0/*IPPROTP_UDP*/);
	if(sd<0)
	{
		perror("socket()");
		exit(1);
	}
	laddr.sin_family=AF_INET;
	laddr.sin_port=htons(atoi(RCVPORT));
	inet_pton(AF_INET,"0.0.0.0",&laddr.sin_addr);
	if(bind(sd,(void*)&laddr,sizeof(laddr))<0)
	{
		perror("bind()");
		exit(1);
	}

	/*!!!!!*/
	raddr_len=sizeof(raddr);

	while(1)
	{
        recvfrom(sd,rbufp,size,0,(void*)&raddr,&raddr_len);
		inet_ntop(AF_INET,&raddr.sin_addr,ipstr,IPSTRSIZE);
		printf("---message from%s:%d------\n",ipstr,ntohs(raddr.sin_port));

		printf("name=%s\n",rbufp->name);
		printf("math=%d\n",ntohl(rbufp->math));
		printf("chinese=%d\n",ntohl(rbufp->chinese));

	}
	close(sd);
	exit(0);
}
