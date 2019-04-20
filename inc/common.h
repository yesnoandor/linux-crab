#ifndef _COMMON_H
#define _COMMON_H

// C++
#include	<cstdlib>
#include	<fstream>
#include	<iostream>
#include	<typeinfo>
#include	<vector>
#include 	<string>
#include 	<map>
#include	<thread>
#include	<functional>	// std::function


// C
#include	<unistd.h>
#include	<errno.h>
#include	<string.h>
#include 	<signal.h>
#include	<sys/wait.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<arpa/inet.h> 


// Libs
#include	"event2/event.h"
#include	"event2/bufferevent.h" 
#include	"event2/buffer.h" 
#include	"event2/util.h"
#include	"event2/thread.h"

#include	"CJsonObject.h"


// User
#include	"debug.h"

#endif
