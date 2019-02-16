#include	<iostream>
#include	<typeinfo>
#include	<vector>

#include	<unistd.h>

#include	"process.h"
#include	"debug.h"

int Process::get_process_id(string name)
{
#if 0
	DIR *pdir = NULL;  
 struct dirent *pde = NULL;  
   
 FILE *pf = NULL;  
 char buff[128];  
 pid_t pid;  
 char szName[128];  
   
 pdir = opendir("/proc");  
 if (!pdir)  
 {  
  perror("open /proc fail.\n");  
  return -1;  
 }  
   
 while ((pde = readdir(pdir)))  
 {  
  if ((pde->d_name[0] < '0')|| (pde->d_name[0] > '9'))  
  {  
   continue;  
  }  
    
  sprintf(buff, "/proc/%s/status", pde->d_name);  
  pf = fopen(buff, "r");  
  if (pf)  
  {  
   fgets(buff,sizeof(buff),pf);  
   fclose(pf);  
     
   sscanf(buff,"%*s %s",szName); 
   pid=atoi(pde->d_name);  
     
   if(strcmp(szName,pName)==0)  
   {  
    closedir(pdir);  
    return pid;  
   }  
  }  
 }  
   
 closedir(pdir);  
#endif 

	return 0;  
}

void add_process(pid_t pid, char cmd[])
{
#if 0
    proc *head = g_root;
    while (head->process_id != 0) {
        head = head->next;
    }
    head->process_id = pid;
    head->cmd = cmd;
    head->is_stop = false;

    head->next = malloc(sizeof(proc));

    if (head->next == NULL) {
        perror("Error: Out of memory");
        return;
    }

    head->next->process_id = 0;
#endif    
}


#if 0
#include <iostream>
#include <vector>
 
using namespace std;
 
typedef struct student{
    int stud_number;
    char stud_name[20];
}stud;
 
typedef std::vector<stud> studentVector;
 
int main(){
    stud jourdon = { 01, "Jourdon" };
    stud mary = { 02, "Mary" };
    stud niki = { 03, "Niki" };
    studentVector studVec;
    studVec.push_back(jourdon);
    studVec.push_back(mary);
    studVec.push_back(niki);
 
    //遍历修改stud_number的值
    studentVector::iterator it;
    for (it = studVec.begin(); it != studVec.end(); it++)
        it->stud_number += + 201600;
 
    //遍历打印
    for (it = studVec.begin(); it != studVec.end(); it++)
        cout << it->stud_number << " " << it->stud_name << endl;
 
}


https://blog.csdn.net/feliciafay/article/details/9128385
#endif



void Process::create_process(string path)
{	
	DbgFuncEntry();

	pid_t pid = fork();
	if (pid == 0)
	{
		if (execv(path.c_str(),(char* const*)0) == -1)
		{
			DbgError("can't run command (%s)!\r\n",path);
		}
		DbgGood("son of process!\r\n");
		exit(0);
	}
	else if (pid > 0)
	{
		process_info_t info;

		DbgGood("pid = %d\r\n",pid);
		
		info.name = "";
		info.path = path;
		info.pid = pid;
		info.status = 1; 
		process_info.push_back(info);
	}
	else
	{
		DbgError("can't fork process!\r\n");
	}

	DbgFuncExit();
}

void Process::kill_process(int pid)
{

}

void Process::kill_process(string name)
{
	DbgFuncEntry();

	DbgFuncExit();
}

void Process::update_process()
{
	DbgFuncEntry();

	DbgFuncExit();
}

void Process::list_process()
{
	process_info_t info;
	
	DbgFuncEntry();
	
	for (process_info_vector::iterator it = process_info.begin();it<process_info.end();++it)  
	{
		info = *it;

		cout << "name = " << info.name << endl;
		cout << "pid = " << info.pid << endl; 		
		cout << "path = " << info.path << endl;
		cout << "status = " << info.status << endl; 
	} 

	DbgFuncExit();
}
   

Process::Process()
{
	DbgFuncEntry();

	DbgFuncExit();

}

Process::~Process()
{

}


