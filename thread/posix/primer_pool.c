#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>

#define LEFT 30000000
#define RIGHT 30000200
#define THRNUM 4

static int num=0;
static pthread_mutex_t mutex_num=PTHREAD_MUTEX_INITIALIZER;

struct thr_arg
{
	int n;
};

static void *thr_prime(void *p)
{
	int i;
	i=((struct thr_arg*)p)->n;
//	free(p);
	int mark=1;
    for(int j=2;j<i/2;j++)
	{
		if(i%j==0)
		{
			mark=0;
			break;
		}
	}
	if(mark)
	   printf("%d is a primer\n",i);

   	pthread_exit(p);		
}

int main()
{
	int i,err;
	pthread_t tid[THRNUM];
	struct thr_arg *p;
	void *ptr;
    
	for(i=LEFT;i<THRNUM;i++)
	{
		p=malloc(sizeof(*p));
		if(p==NULL)
		{
			perror("malloc()");
			exit(1);
		}

		err=pthread_create(tid+i,NULL,thr_prime,(void*)i);
		if(err)
		{
			fprintf(stderr,"pthread_create():%s\n",strerror(err));
			exit(1);
		}
	}

	for(i=LEFT;i<=RIGHT;i++)
	{
	   pthread_join(tid[i-LEFT],&ptr);
	   free(ptr);

	}

	pthread_mutex_destroy(&mut_num);
	
    exit(0);
}


		
