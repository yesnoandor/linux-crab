#ifndef _PARSE_CONFIG_H
#define _PARSE_CONFIG_H

#include "common.h"

using namespace std;

class parseConfig
{
private:
    string m_ip;
    int m_port;

    string m_prefix;
    string m_suffix;
    string m_path;

public:
    parseConfig();
    parseConfig(const char *file);

    void parseConfigXmlFile(const char *file);
    
    string getSocketIp();
    int getSocketPort();

    string getEventLogtPrefix();
    string getEventLogtSuffix();
    string getEventLogtPath();
};

#endif

