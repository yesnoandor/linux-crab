/******************************************************************************

  Copyright (C), 2017-2019, AVS Team Flex SDC Co., Ltd.

 ******************************************************************************
  File Name     : parseXml.cpp
  Version       : Initial Draft
  Author        : David Xu
  Created       : 2019/2/17
  Last Modified :
  Description   : This Class Parse XML then Get Process Configuration
  Function List :
              parseXml.getCommandPath
              parseXml.getProcess
              parseXml.getProcessValue
              parseXml.getReturnValueDescription
              parseXml.parseXml
              parseXml.parseXmlFile
  History       :
  1.Date        : 2019/2/17
    Author      : David Xu
    Modification: Created file

******************************************************************************/

// ----- include file 
#include	"common.h"
#include	"mxml.h"
#include 	"parseXml.h"

// ----- external variables 

// ----- internal variables 

// ----- external routine prototypes 

// ----- internal routine prototypes 

// ----- project-wide global variables 

// ----- module-wide global variables 

// ----- constants 

// ----- macros 

// ----- routines' implementations 



using namespace std;

parseXml::parseXml(const char *file)
{
	parseXmlFile(file);
}


void parseXml::parseXmlFile(const char *file)
{
    FILE *fp;
    mxml_node_t *tree, *node, *root;
    string processName, commandValue;
    
    fp = fopen(file, "r");
    tree = mxmlLoadFile(NULL, fp, MXML_TEXT_CALLBACK);
    fclose(fp);
    
    root = mxmlFindElement(tree, tree, "root", NULL, NULL, MXML_DESCEND);
    for (node = mxmlFindElement(root, tree, "item", NULL, NULL, MXML_DESCEND); node != NULL; node = mxmlFindElement(node, root, "item", NULL, NULL, MXML_NO_DESCEND))
    {
        processName = mxmlElementGetAttr(node, "name");
        configureProcesses.push_back(processName);
        map<int, string> valueMap;
        for (auto valueNode = mxmlFindElement(node, node, "value", NULL, NULL, MXML_DESCEND); valueNode != NULL; valueNode = mxmlFindElement(valueNode, node, "value", NULL, NULL, MXML_NO_DESCEND))
        {
            valueMap.insert({stoi(valueNode->child->value.text.string), mxmlElementGetAttr(valueNode, "result")});
        }
        processMap.insert({processName, valueMap});

        commandValue = mxmlFindElement(node, node, "command", NULL, NULL, MXML_DESCEND)->child->value.text.string;
        commandMap.insert({processName, commandValue});
    }
    
    mxmlDelete(tree);
}

vector<string> parseXml::getProcess()
{
    return configureProcesses;
}


vector<int> parseXml::getProcessValue(string processName)
{
	vector<int> keys;
	map<int,string> maps = processMap[processName];
	map<int,string>::iterator iter;

	for(iter = maps.begin();iter!=maps.end();iter++)
	{
		int value = iter->first;
		keys.push_back(value);
	}

	return keys;
}

string parseXml::getCommandPath(string process)
{
    return commandMap[process];
}

string parseXml::getReturnValueDescription(string process, int key)
{
    return processMap[process][key];
}
