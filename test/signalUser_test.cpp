#include	"common.h"
#include	"signalUser.h"

static void signal_user1_treat(void)
{
	DbgFuncEntry();

	DbgGood("Received SIGUSR1 signal!\r\n");
	
	DbgFuncExit();
}

void signalUser_test(void)
{
	DbgFuncEntry();

	register_signal_user(signal_user1_treat);

	do{
		pause();
		DbgGood("pause is over.\n");
	}while(1);
	
	DbgFuncExit()
}


static void signal_alarm_treat(void)
{
	DbgFuncEntry();

	alarm(4);
	
	DbgGood("Received ALARM signal!\r\n");
	
	DbgFuncExit();
}

void signalAlarm_test(void)
{
	DbgFuncEntry();

	register_signal_alarm(signal_alarm_treat);

	alarm(4);
	
	do{
		pause();
		DbgGood("pause is over.\n");
	}while(1);

	DbgFuncExit()
}

