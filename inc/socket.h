
#ifndef _SOCKET_TCP_H_
#define _SOCKET_TCP_H_

#include	"common.h"

using namespace std;

class socketTcp
{
private:
	thread  taskThread;

	string ip;
	short port;

	struct bufferevent* bev;
	int state;		// =0,disconnected; =1,connected
	int timeout;	// 
	int threhold;

	std::function<void()> mCallback;

private:
	void taskHandle();
	static void aux_server_read_cb(struct bufferevent* bev, void* arg);
	static void aux_server_event_cb(struct bufferevent *bev, short event, void *arg);
	static void socket_timeout_cb(int fd, short event, void * arg);
	
public:
	socketTcp(string ip,short port);
	socketTcp();
	~socketTcp();	

	void start(string ip,short port);
	void send(unsigned char *buff,int len);

	void registerCallback(const std::function<void()> & callback);
};

#endif
