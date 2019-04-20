#include "common.h"
#include "test/parseConfig_test.h"
#include "parseConfig.h"

using namespace std;

void parseConfig_test()
{
	DbgFuncEntry();
	
	parseConfig parse("setting.xml");
	
	string ip = parse.getSocketIp();
	int port = parse.getSocketPort();

	cout << "ip is " << ip << endl;
	cout << "port= " << port << endl;

	string prefix = parse.getEventLogtPrefix();
	string path = parse.getEventLogtPath();
	
	cout << "prefix is " << prefix << endl;
	cout << "path is " << path << endl;

	DbgFuncExit();
}

