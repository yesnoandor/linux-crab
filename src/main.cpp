#include	"common.h"

#include	"test/process_test.h"
#include	"test/parseXml_test.h"
#include	"test/parseConfig_test.h"
#include	"test/signalUser_test.h"
#include	"test/socketTcp_test.h"
#include	"test/parseAresEvent_test.h"

#include	"test/log4cplus_test.h"

#include	"demo/AresDemo.h"
#include	"demo/ZeusDemo.h"

int main(int argc,char* argv[])
{
	//log4cplus_test();
	
	//process_test();
	//parseXml_test();
	//parseConfig_test();
	//signalUser_test();
	//signalAlarm_test();
	//socketTcp_test();
	//parseAresEvent_test();

	
	//AresDemo();

	ZeusDemo();
	
}
