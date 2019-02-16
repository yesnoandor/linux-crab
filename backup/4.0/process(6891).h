
#ifndef _PROCESS_H_
#define _PROCESS_H_

#include	<string>
#include	<vector>

using namespace std;
 
typedef struct{
    string name;
    string path;
    int pid;
	int status;	// =1, is Running;
	int level;	// =0, never exit,default = 10 
}process_info_t;
 
typedef vector<process_info_t> process_info_vector;

class Process
{
private:
	static const int DEFAULT_PROCESS_LEVEL = 10;
	enum { IDLE_STATE = 0,RUNNING_STATE};
	
private:
	process_info_vector process_info;

public:
	enum { NO_EXIT = 0, SELF_EXIT, TERMSIG_EXIT, STOPSIG_EXIT,CONTINUED_EXIT};
		
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
    
private:
	int get_process_id(string path);
	string get_process_path(int pid);
	
	void remove_process_info(int pid);
	void remove_process_info(string path);

	void stop_process_info(int pid);
	void start_process_info(int pid);
};



#endif
