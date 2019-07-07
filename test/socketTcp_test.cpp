#include	"common.h"

#include 	"parseAresEvent.h"
#include	"socket.h"

parseAresEvent mm_parse;

void socketTcp_test(void)
{
	mm_parse.start("mlog20190216.txt");
    //vector<string> event = parse.getAresEvent();

	string str = "hello\r\n";
	socketTcp so("127.0.0.1",8000);

	so.registerCallback(std::bind(&parseAresEvent::onCallback,mm_parse,std::placeholders::_1, std::placeholders::_2));
	
	do{
		sleep(2);
		so.send((unsigned char *)str.c_str(),str.length() + 1);
	}while(1);
	pause();
}
