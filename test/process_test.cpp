#include	<iostream>
#include	<unistd.h>

#include	"process.h"
#include	"debug.h"

using namespace std;

void process_test(void)
{
	DbgFuncEntry();
	
	Process * p = new Process();
	p->create_process("/home/wenyu/work/diagnostic/linux-crab/resource/MonitBase");
	p->create_process("/home/wenyu/work/diagnostic/linux-crab/resource/MonitBase2");
	p->create_process("/home/wenyu/work/diagnostic/linux-crab/resource/MonitBase3");
	
	p->list_process();

	sleep(2);

	do{
		pid_t pid;
		int status;
		int exit_state;

		status = p->update_process(pid,exit_state);
		DbgPrintf("pid = %d\r\n",pid);
		DbgPrintf("status = %d\r\n",status);
		DbgPrintf("exit_state = %d\r\n",exit_state);

		if(status == Process::NO_EXIT)
		{
			break;
		}
		
	}while(1);

	delete p;
	
	sleep(2);
	
	DbgFuncExit();
}


