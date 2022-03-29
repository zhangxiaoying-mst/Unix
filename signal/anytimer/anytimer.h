#ifndef ANYTIMER_H__
#define ANYTIMER_H__

#define JOB_MAX 1024
typedef void at_jobfunc_t(void*);
int at_addjob(int sec,at_jobfunc_t *jobp,void *arg);


#endif
