#ifndef MYTBF_H__
#define MYTBF_H__

#define MYTBF_MAX 1024 //桶的最大值
typedef void mytbf_t;

mytbf_t* mytbf_init(int cps,int burst);//参数：每秒钟传输的字符以及上限

int mytbf_fetchtoken(mytbf_t*,int);//取令牌
int mytbf_returntoken(mytbf_t*,int);

int mytbf_destory(mytbf_t*); //释放空间

#endif

