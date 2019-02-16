#include <iostream>
#include <vector>
#include <string>
#include "parseXml.h"
#include "parseXmlTest.h"
using namespace std;

void printProcessName()
{
    parseXml parse("configure.xml");
    vector<string> processVector = parse.getProcess();
    cout << "process name is :" << endl;
    for (auto it = processVector.begin(); it != processVector.end(); ++it)
    {
        cout << *it << endl;
    }
}

void printCommandPath(string processName)
{
    parseXml parse("configure.xml");
    string commandPath = parse.getCommandPath(processName);
    cout << "process is : " << processName << "\ncommand path is : " << commandPath << endl;
}

void printReturnValueDescription(string processName, int key)
{
    parseXml parse("configure.xml");
    string description = parse.getReturnValueDescription(processName, key);
    cout << "process is : " << processName << "\nthe key is : " << key << "\nthe description is : " << description << endl;
}