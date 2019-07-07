/******************************************************************************

  Copyright (C), 2017-2019, AVS Team Flex SDC Co., Ltd.

 ******************************************************************************
  File Name     : process.h
  Version       : Initial Draft
  Author        : David Xu
  Created       : 2019/2/17
  Last Modified :
  Description   : process.cpp header file
  Function List :
  History       :
  1.Date        : 2019/2/17
    Author      : David Xu
    Modification: Created file

******************************************************************************/

#ifndef _PROCESS_H_
#define _PROCESS_H_

#include	"common.h"

using namespace std;
 
typedef struct{
	string name;
	string path;
	int pid;
	int status;	// =1, is Running;
	int level;		// =0, never exit,default = 10 
	int exit_value;	
}process_info_t;
 
typedef vector<process_info_t> process_info_vector;

class Process
{
private:
	static const int DEFAULT_PROCESS_LEVEL = 10;
	enum { IDLE_STATE = 0,RUNNING_STATE,STOP_STATE,EXIT_STATE};
	
private:
	process_info_vector process_info;

public:
	enum {NO_EXIT = 0, SELF_EXIT, TERMSIG_EXIT, STOPSIG_EXIT,CONTINUED_EXIT};

public:
	Process();
	~Process();

	void create_process(string path);
	void kill_process(int pid);
	void kill_process(string name);
	void stop_process(int pid);
	void stop_process(string name);
	void start_process(int pid);
	void start_process(string name);
	
	int update_process(int& pid,int& exit_value);
	void list_process();

	int get_process_count();
	int get_running_process_count();
	int get_stop_process_count();
	int get_exit_process_count();

	int get_exit_value(string name);
	
private:
	int get_process_id(string path);
	string get_process_path(int pid);
	
	void remove_process_info(int pid);
	void remove_process_info(string path);

	void stop_process_info(int pid);
	void start_process_info(int pid);

	void exit_process_info(int pid,int value);
};



#endif
