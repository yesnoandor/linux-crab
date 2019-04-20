#include	"common.h"

#include	"socket.h"

using namespace std;

void socketTcp::send(unsigned char *buff,int len)
{
	DbgFuncEntry();

	#if 0
	{
		int i;
		DbgPrintf("len = %d\r\n",len);
		for(i =0;i<len;i++)
		{
			DbgPrintf("buff[%d] = 0x%x\r\n",i,buff[i]);
		}
	}
	#endif

	if(this->state)
	{
		bufferevent_write(this->bev,buff,len);
	}
	else
	{
		DbgPrintf("socket is not ready!\r\n");
	}
	
	DbgFuncExit();
}

void socketTcp::socket_timeout_cb(int fd, short event, void * arg)
{
	socketTcp * socket;
	struct evbuffer *input;
		
	DbgFuncEntry();

	socket = (socketTcp *)arg;

	socket->timeout++;
	if(socket->timeout >= socket->threhold)
	{
		socket->bev = NULL;
		socket->state = 0;

		DbgWarn("socket transfer timeout\r\n");	
	}

#if 0
	long i;
	struct bufferevent* bev;
	struct event_base* base = (struct event_base*)pArg;

	DbgPrintf("base = 0x%x\r\n",base);

	for(i=0;i<MAX_SOCKET_NUM;i++)
	{
		if((strcmp(server_ip[i],"") == 0) || (server_port[i] == 0))
		{
			continue;
		}

		// 超时保护
		socket_mngr_list[i].timeout++;
		if(socket_mngr_list[i].timeout >= socket_mngr_list[i].threhold)
		{
			socket_mngr_list[i].bev = NULL;
			socket_mngr_list[i].state = 0;

			DbgWarn("socket transfer timeout\r\n");	
		}

		// 重连机制
		if(0 == socket_mngr_list[i].state)
		{
			DbgPrintf("re-connect socket ip = %s port = %d\r\n",server_ip[i],server_port[i]);
			
			// 创建一个bufferevent
			// BEV_OPT_CLOSE_ON_FREE : 当 bufferevent 被释放同时关闭底层
			bev = bufferevent_socket_new(base,-1,BEV_OPT_CLOSE_ON_FREE);	
				
			// 设置读取方法和error 时候的方法  
			bufferevent_setcb(bev, server_read_cb[i], NULL, server_event_cb[i], (void *)i); 
			// 设置类型  
			//bufferevent_enable(bev, EV_READ | EV_PERSIST);
		
			struct sockaddr_in tSockAddr;
			memset(&tSockAddr, 0, sizeof(tSockAddr));
			tSockAddr.sin_family = AF_INET;
			tSockAddr.sin_addr.s_addr = inet_addr(server_ip[i]);
			tSockAddr.sin_port = htons(server_port[i]);
		
			// 连接服务器
			if( bufferevent_socket_connect(bev, (struct sockaddr*)&tSockAddr, sizeof(tSockAddr)) < 0)
			{
				DbgError("connect to server error(ip = %s,port = %d) failed!\r\n",server_ip[i],server_port[i]); 
			}
		}
		else
		{
			#if 0
			// 心跳机制
			switch (i)
			{
				case MAIN_SOCKET:
					gb905_heart_beat_send();
					break;

				default:
					break;
			}
			#endif
		}
	
	}
#endif	
	DbgFuncExit();
}

void socketTcp::aux_server_read_cb(struct bufferevent* bev, void* arg)  
{
	long index;
	//unsigned char msg[1024];
	
	array<char, 1024> msg;
	int len =0;
	int offset=0;

	socketTcp * socket;
	struct evbuffer *input;

	DbgFuncEntry();
	
	socket = (socketTcp *)arg;

	//len = bufferevent_read(bev, msg, sizeof(msg));

	//index = (long)arg;
	//DbgPrintf("index = %d\r\n",index);

	input = bufferevent_get_input(bev);
	len = evbuffer_copyout(input, msg.data(), msg.size());

	#if 0
	{
		int i;
		DbgPrintf("len = %d\r\n",len);
		for(i =0;i<len;i++)
		{
			DbgPrintf("msg[%d] = 0x%x\r\n",i,msg.at(i));
		}
	}
	#endif

	offset = len;
	//offset = gb905_protocol_ayalyze(msg,len);
	evbuffer_drain(input,offset);
	DbgPrintf("msg = %s\r\n",msg.data());

	socket->mCallback();
	
	//socket_mngr_list[index].timeout = 0;

	//DbgPrintf("offset = %d\r\n",offset);

	DbgFuncExit(); 
}

void socketTcp::aux_server_event_cb(struct bufferevent *bev, short event, void *arg)  
{
	socketTcp * socket;
		
	DbgFuncEntry();
	
	socket = (socketTcp *)arg;
	
		
	if( BEV_EVENT_CONNECTED == event )
	{
		bufferevent_enable( bev, EV_READ | EV_PERSIST);

		socket->bev = bev;
		socket->timeout = 0;
		socket->threhold = 10;
		socket->state = 1;
		
		//socket_mngr_list[index].bev = bev;
		//socket_mngr_list[index].timeout = 0;
		//socket_mngr_list[index].threhold = 10;
		
		//socket_mngr_list[index].state = 1;

		DbgGood("aux server connect success!\r\n");
	}
	else
	{
		if (event & BEV_EVENT_EOF)
			DbgPrintf("connection closed\r\n");
		else if (event & BEV_EVENT_ERROR)
			DbgPrintf("some other error\n");
	
		
		bufferevent_free(bev);

		socket->bev = NULL;
		socket->state = 0;
		
		//socket_mngr_list[index].bev = NULL;
		//socket_mngr_list[index].state = 0;
	}

	DbgFuncExit();
}


void socketTcp::start(string ip,short port)
{
	DbgFuncEntry();
	
	this->ip = ip;
	this->port = port;

	if (taskThread.get_id() == std::thread::id())
	{
		taskThread = std::thread(&socketTcp::taskHandle,this);
	}

	DbgFuncExit();
}


void socketTcp::taskHandle()
{
	struct bufferevent* bev;
	struct event_base* base;
		
	DbgFuncEntry();

	evthread_use_pthreads();	// supported mulit-thread
	base = event_base_new();	
	evthread_make_base_notifiable(base);
	
	bev = bufferevent_socket_new(base,-1,BEV_OPT_CLOSE_ON_FREE);
	bufferevent_setcb(bev, &socketTcp::aux_server_read_cb, NULL, &socketTcp::aux_server_event_cb,(void *)this); 
	// ��������  
	//bufferevent_enable(bev, EV_READ | EV_PERSIST);
	
	struct sockaddr_in tSockAddr;
	memset(&tSockAddr, 0, sizeof(tSockAddr));
	tSockAddr.sin_family = AF_INET;
	tSockAddr.sin_addr.s_addr = inet_addr(this->ip.c_str());
	tSockAddr.sin_port = htons(this->port);
	
	if(bufferevent_socket_connect(bev, (struct sockaddr*)&tSockAddr, sizeof(tSockAddr)) < 0)
	{
		DbgError("connect to server error(ip = %s,port = %d) failed!\r\n",this->ip,this->port); 
		return;
	}
	
	// timer event
	struct event *timeout = NULL;
	struct timeval tv = {10, 0};
	
	timeout = event_new(base, -1, EV_PERSIST, socket_timeout_cb, base);
	evtimer_add(timeout, &tv);

	#if 0
	struct event *pipe_event = NULL;
	
	if(pipe(pipe_fd))
	{
		DbgError("create pipe failed!\r\n");
		return ((void *)-1);;
	}
	
	DbgGood("open pipe success!\r\n");

	pipe_event = event_new(base,
						   pipe_fd[0],
						   EV_READ | EV_PERSIST,
						   pipe_read_cb,
						   NULL);
	event_add(pipe_event, NULL);
	#endif	
	
	#if 0
	struct event* ev_cmd = event_new(base, 
									 STDIN_FILENO,
									 EV_READ | EV_PERSIST,
									 cmd_msg_cb,
									 (void*)bev);
	event_add(ev_cmd, NULL);
	#endif
		

	event_base_dispatch(base);
	
	evtimer_del(timeout);
		
	bufferevent_free(bev);
	
	event_base_free(base);
	
	DbgFuncExit();
}

void socketTcp::registerCallback(const std::function<void()> & callback)
{
	mCallback = callback;
}



#if 0
void socketTcp::register(const std::function<void()> & callback)
{
	
}
#endif

socketTcp::socketTcp(string ip,short port)
{
	DbgFuncEntry();

	start(ip,port);
	
	DbgFuncExit();
}

socketTcp::socketTcp()
{
	DbgFuncEntry();	
	DbgFuncExit();
}

socketTcp::~socketTcp()
{
	DbgFuncEntry();
	
	DbgFuncExit();
}

