#include	"socket.h"

void socketTcp_test(void)
{
	string str = "hello";
	socketTcp so("127.0.0.1",8888);

	do{
		sleep(2);
		so.send((unsigned char *)str.c_str(),str.length() + 1);
	}while(1);
	pause();
}
