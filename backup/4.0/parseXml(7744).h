#ifndef MONIT_PARSER_H
#define MONIT_PARSER_H
#include <vector>
#include <map>
#include <mxml.h>
using namespace std;

class parseXml
{
  private:
    FILE *fp;
    mxml_node_t *tree, *node, *root;
    string processName, commandValue;
    vector<string> configureProcesses;
    map<string, map<int, string>> processMap;
    map<string, string> commandMap;

  public:
    parseXml();
    parseXml(const char *file);
    vector<string> getProcess();
    string getCommandPath(string process);
    string getReturnValueDescription(string process, int key);
};
#endif // !MONIT_PARSER_H