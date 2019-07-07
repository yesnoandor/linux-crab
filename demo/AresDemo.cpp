#include	"common.h"

#include	"socket.h"
#include	"parseConfig.h"
#include	"signalUser.h"
#include 	"parseAresEvent.h"


//parseAresEvent m_parse;
static socketTcp m_socket;
parseAresEvent m_parse;
string m_file;


string getEventLogName(string path,string prefix,string suffix)
{
	char str_buf[128] = {0};
	string file;

	//file = "mlog20190216.txt";

	time_t timep;
	struct tm *p;

	DbgFuncEntry();
	
	time(&timep);
	p = localtime(&timep);

	cout << "year = " << p->tm_year + 1900 << endl;
	cout << "month = " << p->tm_mon + 1 << endl;
	cout << "day = " << p->tm_mday << endl;

	sprintf(str_buf,"%04d%02d%02d",p->tm_year + 1900,p->tm_mon+1,p->tm_mday);

	file = path + '/' + prefix;
	file.append(str_buf);
	file += '.';
	file.append(suffix);

	cout << "log file name = " << str_buf << endl;
	cout << "file name = " << file << endl;

	DbgFuncExit();

	//file = "mlog20190216.txt";

	return file;
	
#if 0
	if ((dir = opendir(base_path)) == NULL)
	{
		printf("Open dir(%s) error...\r\n",base_path);
		exit(1);
	}

	while ((ptr = readdir(dir)) != NULL)
	{
		if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0)	///current dir OR parrent dir
			continue;
		else if(ptr->d_type == DT_REG)
		{
			//printf("this is a regular file,d_name:%s/%s\n",base_path,ptr->d_name);
		}
		else if(ptr->d_type == DT_DIR)		// --> dir
		{
			//printf("this is a dir,d_name:%s/%s\n",base_path,ptr->d_name);
		}
		else if(ptr->d_type == DT_LNK)		// --> link file
		{
			//printf("this is a link,d_name:%s/%s\n",base_path,ptr->d_name);
		}
		else if(ptr->d_type == DT_CHR)		// --> file
		{
			//printf("this is a char device,d_name:%s/%s\n",base_path,ptr->d_name);

			char result_name[20] = {0};
			static char video_name[256] = {0};
			if (reg_match(ptr->d_name,"video[0-9]+",result_name) == 0)
			{
				printf("this is a video device,d_name:%s/%s\n",base_path,ptr->d_name);
				printf("result_name:%s/%s\n",base_path,result_name);

				sprintf(video_name,"%s/%s",base_path,result_name);

				printf("video_name:%s\r\n",video_name);
				fd = open_device(video_name);
				if (fd != -1)
				{
					if(match_device(fd))
					{
						strcpy(dev_name,video_name);
						ret = true;
						close(fd);
						break;
					}
					close(fd);
				}
			}
		}
	}

	closedir(dir);

	printf("dev_name:%s\r\n",dev_name);
#endif	
}



static void signal_treat(void)
{
	DbgFuncEntry();
	
	string eventJson;
	//parseConfig config("setting.xml");
	
	//string ip = config.getSocketIp();
	//int port = config.getSocketPort();
	//socketTcp so(ip,port);

	//string prefix = config.getEventLogtPrefix();
	//string path = config.getEventLogtPath();

	//string file = getEventLogName(path);
	//cout << "event file name = " << file << endl;
	
	//parseAresEvent parse(file);
	m_parse.start(m_file.c_str());

	vector<string> event = m_parse.getNewAresEvent();

	cout << "size =" << event.size() << endl;
	
	m_socket.registerCallback(std::bind(&parseAresEvent::onCallback,m_parse,std::placeholders::_1, std::placeholders::_2));
	for (auto it = event.begin(); it != event.end(); ++it)
	{
		eventJson = *it;
		cout << "item : " << eventJson << endl;
		m_socket.send((unsigned char *)eventJson.c_str(),eventJson.length() + 1);
	}

	DbgFuncExit();
}

static void signal_user1_treat(void)
{
	DbgFuncEntry();

	DbgGood("Received SIGUSR1 signal!\r\n");
	signal_treat();
	
	DbgFuncExit();
}

static void signal_alarm_treat(void)
{
	DbgFuncEntry();

	DbgGood("Received ALARM signal!\r\n");

	signal_treat();
	
	alarm(300);
	
	DbgFuncExit();
}




void AresDemo(void)
{
	register_signal_user(signal_user1_treat);
	register_signal_alarm(signal_alarm_treat);

	parseConfig config("setting.xml");
	string ip = config.getSocketIp();
	int port = config.getSocketPort();

	cout << "ip is " << ip << endl;
	cout << "port is " << port << endl;
	
	//socketTcp so(ip,port);
	m_socket.start(ip,port);

	string prefix = config.getEventLogtPrefix();
	string suffix = config.getEventLogtSuffix();
	string path = config.getEventLogtPath();

	m_file = getEventLogName(path,prefix,suffix);
	cout << "event file name = " << m_file << endl;
	m_parse.start(m_file.c_str());

	//string prefix = config.getEventLogtPrefix();
	//string path = config.getEventLogtPath();

	//string file = getEventLogName(path);
	//cout << "event file name = " << file << endl;
	
	//parseAresEvent parse(file);
	//m_parse = parse;
    //vector<string> event = parse.getAresEvent();

	//so.registerCallback(std::bind(&parseAresEvent::onCallback, m_parse));
    //for (auto it = event.begin(); it != event.end(); ++it)
    //{
    //	eventJson = *it;
    //   	cout << "item : " << eventJson << endl;
    //   	so.send((unsigned char *)eventJson.c_str(),eventJson.length() + 1);
    //}


	alarm(3);
	
	do{
		pause();
		DbgGood("pause is over.\n");
	}while(1);

}


