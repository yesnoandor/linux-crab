#ifndef _PARSE_XML_H
#define _PARSE_XML_H

#include <vector>
#include <map>
//#include <mxml.h>

using namespace std;

class parseXml
{
private:
    vector<string> configureProcesses;
    map<string, map<int, string>> processMap;
    map<string, string> commandMap;

public:
    parseXml();
    parseXml(const char *file);

    void parseXmlFile(const char *file);
    vector<int> getProcessValue(string processName);
    vector<string> getProcess();
    string getCommandPath(string process);
    string getReturnValueDescription(string process, int key);
};

#endif // !MONIT_PARSER_H