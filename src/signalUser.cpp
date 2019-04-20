

#include	"common.h"
#include	"signalUser.h"


static signal_func_t signal_user;
static signal_func_t signal_alarm;


static void signal_handler(int signo)
{
	DbgFuncEntry();

	switch (signo)
	{
		case SIGUSR1:
			DbgGood("get SIGUSR1!\r\n");
			(*signal_user)();
			break;

		case SIGUSR2:
			break;

		case SIGALRM:
			(*signal_alarm)();
			break;

		default:
			break;
	}

	DbgFuncExit();
	
}

void register_signal_user(signal_func_t func)
{
	DbgFuncEntry();

	if(signal(SIGUSR1, signal_handler) == SIG_ERR)
	{
		DbgError("register SIGUSR1 failed!\r\n");
	}

	signal_user = func;
	
	DbgFuncExit();
}


void register_signal_alarm(signal_func_t func)
{
	DbgFuncEntry();

	if(signal(SIGALRM, signal_handler) == SIG_ERR)
	{
		DbgError("register SIGALRM failed!\r\n");
	}

	signal_alarm = func;
	
	DbgFuncExit();
}

