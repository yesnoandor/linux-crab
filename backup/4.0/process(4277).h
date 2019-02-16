
#ifndef _PROCESS_H_
#define _PROCESS_H_

#include	<string>
#include	<vector>

using namespace std;
 
typedef struct{
    string name;
    string path;
    int pid;
	int status;
}process_info_t;
 
typedef vector<process_info_t> process_info_vector;

class Process
{
private:
	process_info_vector process_info;

public:
	Process();
	~Process();
	
    void create_process(string path);
	void kill_process(int pid);
	void kill_process(string name);
    void update_process();
    void list_process();
    
private:
	int get_process_id(string name);
	
};



#endif
