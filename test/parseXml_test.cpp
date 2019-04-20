
#include "common.h"
#include "parseXml.h"

using namespace std;

void parseXml_test()
{
	string processName;
	string commandPath;
	
	parseXml parse("/home/wenyu/work/diagnostic/linux-crab/resource/common/configure.xml");
    vector<string> processVector = parse.getProcess();

    for (auto it = processVector.begin(); it != processVector.end(); ++it)
    {
    	processName = *it;
       	cout << "process name is : " << processName << endl;

		commandPath = parse.getCommandPath(processName);
    	cout << "command path is : " << commandPath << endl;

    	vector<int> keys =  parse.getProcessValue(processName);
    	for (vector<int>::iterator it = keys.begin();it<keys.end();++it)  
		{
			string desc = parse.getReturnValueDescription(processName,*it);
			
			DbgGood("<key,desc> = <%d,%s>\r\n",*it,desc.c_str());
		}
    }

    
}
