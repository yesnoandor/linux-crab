int Process::get_process_id(std::string process_name)
{
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
 return 0;  
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
