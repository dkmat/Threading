#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"

/*
 * Frequency of preemption
 * 100Hz is 100 times per second
 */
#define _XOPEN_SOURCE 700
#define HZ 100

void sig_handler(int signum){
	(void)signum;
	uthread_yield();
}
sigset_t signal_set;

void preempt_disable(void)
{
	sigemptyset(&signal_set);
	sigaddset(&signal_set,SIGVTALRM);
	sigprocmask(SIG_BLOCK, &signal_set, NULL);
}

void preempt_enable(void)
{
	sigemptyset(&signal_set);
	sigaddset(&signal_set,SIGVTALRM);
	sigprocmask(SIG_UNBLOCK, &signal_set, NULL);
}

void preempt_start(bool preempt)
{
	if(preempt){
		struct sigaction sig;
		struct sigaction old_sig;
		sig.sa_handler = sig_handler;
		sigaction(SIGVTALRM,&sig,&old_sig);
		struct itimerval timer;
		timer.it_interval.tv_sec = 0;
		timer.it_interval.tv_usec = 1000/HZ;
		timer.it_value = timer.it_interval;
		setitimer(ITIMER_VIRTUAL,&timer,NULL);
	}
}

void preempt_stop(void)
{
	sigaction(SIGVTALRM,NULL,NULL);
	setitimer(ITIMER_VIRTUAL,NULL,NULL);
}

