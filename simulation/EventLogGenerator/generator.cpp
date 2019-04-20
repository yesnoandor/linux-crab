
#include <unistd.h>

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

#include "generator.h"

generator::generator()
{
    time_t nowtime = time(NULL);
    char timeArray[15];
    strftime(timeArray, sizeof(timeArray), "%Y%m%d", localtime(&nowtime));
    string timetmp = timeArray;
    outfile.open("mlog" + timetmp + ".txt", ios::out | ios::trunc);
}

generator::~generator()
{
}

void generator::generate(int lineNumber, int interval)
{
    time_t nowtime;
    char timeArray[15];
    for (int i = 1; i < lineNumber; ++i)
    {
        sleep(interval);
        nowtime = time(NULL);
        strftime(timeArray, sizeof(timeArray), "%Y%m%d%H%M%S", localtime(&nowtime));
        outfile << "id=" << i << " date=" << timeArray << " mod=" << mod[random() % 10] << " error=" << error[random() % 10] << endl;
		system("./notify_flex.sh");
    }
    outfile.close();
}
