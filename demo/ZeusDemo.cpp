#include	"common.h"

#include	"process.h"
#include "parseXml.h"
#include	"parseConfig.h"
#include	"socket.h"

void ZeusDemo(void)
{
	DbgFuncEntry();
	
	// 从配置文件获得测试进程信息
	string processName;
	string commandPath;
	
	parseXml parse(PROCESS_CONFIG_FILE_PATH);
	vector<string> processVector = parse.getProcess();

	#if 0
	for (vector<string>::iterator  it = processVector.begin(); it != processVector.end(); ++it)
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
	#endif

	// 进程管理注册各个测试进程
	Process * p = new Process();
	for (vector<string>::iterator it = processVector.begin(); it != processVector.end(); ++it)
	{
		processName = *it;
		commandPath = parse.getCommandPath(processName);
		cout << "process name is : " << processName << endl;
		cout << "command path is : " << commandPath << endl;
		
		p->create_process(commandPath);
	}

	// 整个测试程序的运行
	p->list_process();
	do{
		pid_t pid;
		int status;
		int exit_state;

		status = p->update_process(pid,exit_state);
		DbgPrintf("pid = %d\r\n",pid);
		DbgPrintf("status = %d\r\n",status);
		DbgPrintf("exit_state = %d\r\n",exit_state);
		
		sleep(1);
		if(p->get_running_process_count())
		{
			p->list_process();
		}
		else
		{
			DbgPrintf("no process,exit loop!\r\n");
			break;
		}
		
	}while(1);

	
	// 构造json  格式的测试结果
	vector<string> event;
	string eventJson;
	
	CJsonObject json;
	
	for (vector<string>::iterator it = processVector.begin(); it != processVector.end(); ++it)
	{
		int exitValue;
		string desc;
		
		processName = *it;
		exitValue = p->get_exit_value(processName);
		desc = parse.getReturnValueDescription(processName,exitValue);

		cout << "process name : " << processName << endl;
		cout << "exit value  : " << exitValue << endl;
		cout << "return desc  : " << desc << endl;

		json.Clear();
		json["event"].Add("id","1");
		json["event"].Add("date","20190216092812");
		json["event"].Add("mod",processName);
		if(exitValue)
		{
			json["event"].Add("error",desc);
		}
		else
		{
			json["event"].Add("info",desc);
		}
		
		eventJson = json["event"].ToString();
		cout << "eventJson = " << eventJson << endl;

		event.push_back(eventJson);
	}
	
	// 获得网络传输参数
	parseConfig config(PROCESS_SETTING_FILE_PATH);
	string ip = config.getSocketIp();
	int port = config.getSocketPort();

	cout << "ip is " << ip << endl;
	cout << "port is " << port << endl;

	// 传输测试结果给PC
	socketTcp socket;
	socket.start(ip,port);
	sleep(2);
	
	for (vector<string>::iterator it = event.begin(); it != event.end(); ++it)
	{
		eventJson = *it;
		cout << "item : " << eventJson << endl;
		socket.send((unsigned char *)eventJson.c_str(),eventJson.length() + 1);
	}
	//socket.send((unsigned char *)eventJson.c_str(),eventJson.length() + 1);

	sleep(2);
	socket.stop();
	sleep(5);
	
	DbgFuncExit();
}
