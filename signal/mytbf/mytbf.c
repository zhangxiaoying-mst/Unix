#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<errno.h>


#include"mytbf.h"

static struct mytbf_st* job[MYTBF_MAX];
static int inited=0;

typedef void (*sighandler_t)(int);
static sighandler_t alrm_handler_save;

struct mytbf_st
{
	int cps;//传输速率
	int burst;//上限
	int token;//令牌
	int pos;
	
};

static void alrm_handler(int s)
{
	alarm(1);
	for(int i=0;i<MYTBF_MAX;i++)
	{
		if(job[i]!=NULL)
		{
			job[i]->token+=job[i]->cps;
			if(job[i]->token>job[i]->burst)
				job[i]->token=job[i]->burst;
		}
	}

}

static void module_unload(void)
{
	signal(SIGALRM,alrm_handler_save);
	alarm(0);

	for(int i=0;i<MYTBF_MAX;i++)
	{
		free(job[i]);
	}
}
static void module_load()
{
	alrm_handler_save=signal(SIGALRM,alrm_handler);
	alarm(1);

	atexit(module_unload);//模块卸载出错时，通过钩子函数通过逆序盛名顺序来调用
}

static int get_free_pos(void)
{
	for(int i=0;i<MYTBF_MAX;i++)
	{
		if(job[i]==NULL)
		{
			return i;
		}
	}
	return -1;
}

mytbf_t* mytbf_init(int cps,int burst)//参数：每秒钟传输的字符以及上限
{
	struct mytbf_st *me;
	int pos;

	if(!inited)
	{
    	module_load();
    	inited=1;
	}

	pos=get_free_pos();

	if(pos<0)
	{
		return NULL;
	}

	me=malloc(sizeof(*me));

	if(me==NULL)
		return NULL;

	me->token=0;
	me->cps=cps;
	me->burst=burst;
	me->pos=pos;

	job[pos]=me;

	return me;
}

static int min(int a,int b)
{
	if(a<b) return a;
	else return b;
}

int mytbf_fetchtoken(mytbf_t *ptr,int size)//取令牌
{
	struct mytbf_st *me=ptr;
	int n;

	if(size<=0)
	{
		return -EINVAL;
	}

	while(me->token<=0)
		pause();
	
	n=min(me->token,size);
	me->token-=n;

	return n;



}
int mytbf_returntoken(mytbf_t* ptr,int size)
{
	struct mytbf_st *me=ptr;
	if(size<=0)
	{
		return -EINVAL;
	}

	me->token+=size;
	if(me->token>me->burst)
	{
		me->token=me->burst;
	}

	return size;
}

int mytbf_destory(mytbf_t *ptr)
{
	struct mytbf_st *me=ptr;
	job[me->pos]=NULL;
	free(me);

	return 0;
}




