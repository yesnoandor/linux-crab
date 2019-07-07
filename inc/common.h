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
#include	<stdio.h>
#include	<stdlib.h>
#include	<fcntl.h>
#include	<unistd.h>
#include	<string.h>
#include 	<signal.h>
#include	<errno.h>
#include	<sys/wait.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<arpa/inet.h> 

#include	<sys/types.h>
#include	<sys/stat.h>
#include	<sys/ioctl.h>

// Linux
#include 	<linux/types.h>
#include	<linux/i2c.h>
#include	<linux/i2c-dev.h>

// Libs
#include	"event2/event.h"
#include	"event2/bufferevent.h" 
#include	"event2/buffer.h" 
#include	"event2/util.h"
#include	"event2/thread.h"

#include	"CJsonObject.h"

#include	"mxml.h"

// User
#include	"misc.h"
#include	"node.h"
#include	"error.h"
#include	"debug.h"

#define	PROCESS_CONFIG_FILE_PATH	"./configure.xml"
#define	PROCESS_SETTING_FILE_PATH	"./setting.xml"

#endif
