

#include	"common.h"

#include	"lib/mxml.h"

#include 	"parseXml.h"

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
