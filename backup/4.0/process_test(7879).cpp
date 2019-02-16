#include	<iostream>

#include	"process.h"
#include	"debug.h"

using namespace std;

void process_test(void)
{
	DbgFuncEntry();

	Process * p = new Process();
	p->create_process("hello");

	DbgFuncExit();
}


