#include	"common.h"

#include	"process.h"

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


string Process::get_process_path(int pid)
{
	process_info_vector::iterator it;

	process_info_t info;
	string path;
	
	DbgFuncEntry();
	
	for (it = process_info.begin();it<process_info.end();++it)  
	{
		info = *it;
		if(pid == info.pid)
		{
			path = info.path;
			break;
		}
	}
	
	DbgFuncExit();

	return path;
}


void Process::stop_process_info(int pid)
{
	process_info_vector::iterator it;

	process_info_t info;
	
	DbgFuncEntry();
	
	for (it = process_info.begin();it<process_info.end();++it)  
	{
		info = *it;
		if(pid == info.pid)
		{
			info.status = IDLE_STATE;
			break;
		}
	}
	
	DbgFuncExit();
}

void Process::start_process_info(int pid)
{
	process_info_vector::iterator it;

	process_info_t info;
	
	DbgFuncEntry();
	
	for (it = process_info.begin();it<process_info.end();++it)  
	{
		info = *it;
		if(pid == info.pid)
		{
			info.status = RUNNING_STATE;
			break;
		}
	}
	
	DbgFuncExit();
}


void Process::remove_process_info(int pid)
{
	process_info_vector::iterator it;

	process_info_t info;
	
	DbgFuncEntry();
	
	for (it = process_info.begin();it<process_info.end();++it)  
	{
		info = *it;
		if(pid == info.pid)
		{
			process_info.erase(it);
			break;
		}
	}
	
	DbgFuncExit();
}


void Process::remove_process_info(string path)
{
	process_info_vector::iterator it;
	
	process_info_t info;
		
	DbgFuncEntry();
		
	for (it = process_info.begin();it<process_info.end();++it)	
	{
		info = *it;
		if(path == info.path)
		{
			process_info.erase(it);
			break;
		}
	}
		
	DbgFuncExit();
}

void Process::create_process(string path)
{	
	DbgFuncEntry();

	pid_t pid = fork();
	if (pid == 0)
	{
		if (execv(path.c_str(),(char* const*)0) == -1)
		{
			DbgError("can't run command (%s)!(errno = %s)\r\n",path.c_str(),strerror(errno));
		}
		exit(5);
	}
	else if (pid > 0)
	{
		process_info_t info;
		int pos = path.find_last_of('/');
		
		info.name = path.substr(pos + 1);
		info.path = path;
		info.pid = pid;
		info.status = RUNNING_STATE; 
		info.level = DEFAULT_PROCESS_LEVEL;
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
	DbgFuncEntry();
	
	int state = kill(pid, SIGTERM);
		
	if (state == -1)
	{
		DbgError("kill process %d failed! (%s)\r\n", pid ,strerror(errno));  	
	}

	DbgFuncExit();
}

void Process::kill_process(string path)
{
	pid_t pid ;
	
	DbgFuncEntry();

	pid = get_process_id(path);
	kill_process(pid);
	
	DbgFuncExit();
}

void Process::stop_process(int pid)
{
	int state;
	
	DbgFuncEntry();

	state = kill(pid, SIGSTOP);
	if(state == -1)
	{
		DbgError("stop process %d failed! (%s)\r\n", pid ,strerror(errno));  	
	}

	DbgFuncExit();
}

void Process::stop_process(string path)
{
	pid_t pid;
	
	DbgFuncEntry();

	pid = get_process_id(path);
	stop_process(pid);

	DbgFuncExit();
}

void Process::start_process(int pid)
{
	int state;

	DbgFuncEntry();
	
	state = kill(pid, SIGCONT);
	if(state == -1)
	{
		DbgError("continue process %d failed! (%s)\r\n", pid ,strerror(errno)); 	
	}

	DbgFuncExit();
}

void Process::start_process(string path)
{
	pid_t pid;
	
	DbgFuncEntry();

	pid = get_process_id(path);
	start_process(pid);

	DbgFuncExit();
}


int Process::update_process(int& pid,int& exit_value)
{
	int child_status;
	int status;

	DbgFuncEntry();

	pid = waitpid(-1, &child_status, WNOHANG);
	if (pid == 0)
	{
		status = NO_EXIT;
		exit_value = 0;
		
		DbgPrintf("no child process exit!\r\n");
	}
	else if(pid > 0)
	{
		if (WIFEXITED(child_status))
		{
			status = SELF_EXIT;
			exit_value = WEXITSTATUS(child_status);

			remove_process_info(pid);
			DbgPrintf("process %d exit, status=%d \r\n",pid,exit_value);
		}
		else if (WIFSIGNALED(child_status))
		{
			status = TERMSIG_EXIT;
			exit_value = WTERMSIG(child_status);

			DbgPrintf("process %d is killed by signal %d \r\n", pid,exit_value);
		
			string path = get_process_path(pid);
			remove_process_info(pid);

			create_process(path);	
		}
		else if (WIFSTOPPED(child_status))
		{
			status = STOPSIG_EXIT;
			exit_value = WSTOPSIG(child_status);
			stop_process_info(pid);
			
			DbgPrintf("process %d is stopped by signal %d \r\n", pid,exit_value);
		}
		else if (WIFCONTINUED(child_status))
		{
			status = CONTINUED_EXIT;
			exit_value = 0;
			start_process_info(pid);
			
			DbgPrintf("process %d is continued\r\n", pid);			
		}
	}
	else // if (pid == -1)
	{
		DbgError("call waitpid() function error! (%s)\r\n",strerror(errno));  
		exit(EXIT_FAILURE);
	}
	
	
	DbgFuncExit();

	return status;
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
		cout << "level = " << info.level << endl;
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
	DbgFuncEntry();

	DbgFuncExit();
}


