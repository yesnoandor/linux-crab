#include	"common.h"
#include	"mxml.h"
#include 	"parseConfig.h"

using namespace std;

parseConfig::parseConfig(const char *file)
{
	parseConfigXmlFile(file);
}


void parseConfig::parseConfigXmlFile(const char *file)
{
    FILE *fp;
    mxml_node_t *tree, *node, *root;
    string item, ipType, portType,nodeType,nodeName;

	DbgFuncEntry();

	DbgPrintf("file = %s\r\n",file);
	
    fp = fopen(file, "r");
    tree = mxmlLoadFile(NULL, fp, MXML_TEXT_CALLBACK);
    fclose(fp);

    root = mxmlFindElement(tree, tree, "root", NULL, NULL, MXML_DESCEND);
    for (node = mxmlFindElement(root, tree, "item", NULL, NULL, MXML_DESCEND); node != NULL; node = mxmlFindElement(node, root, "item", NULL, NULL, MXML_NO_DESCEND))
    {
        item = mxmlElementGetAttr(node, "name");
        if (item == "socket")
        {
            auto ipNode = mxmlFindElement(node, node, "data", NULL, NULL, MXML_DESCEND);
            ipType = mxmlElementGetAttr(ipNode, "type");
            if (ipType == "STRING")
            {
                m_ip = ipNode->child->value.text.string;
            }
            
            auto portNode = mxmlFindElement(ipNode, node, "data", NULL, NULL, MXML_NO_DESCEND);
            portType = mxmlElementGetAttr(portNode, "type");
            if (portType == "DWORD")
            {
                m_port = atoi(portNode->child->value.text.string);
            }
        }

		
        if(item == "event_file")
        {
        	auto prefixNode = mxmlFindElement(node, node, "data", NULL, NULL, MXML_DESCEND);
            nodeType = mxmlElementGetAttr(prefixNode, "type");
            nodeName = mxmlElementGetAttr(prefixNode, "name");

			cout << "node name = " << nodeName << endl;
			cout << "node type = " << nodeType << endl;
			
            if (nodeType == "STRING")
            {
                m_prefix = prefixNode->child->value.text.string;
            }

			
			auto suffixNode = mxmlFindElement(prefixNode, node, "data", NULL, NULL, MXML_NO_DESCEND);
			nodeType = mxmlElementGetAttr(suffixNode, "type");
			nodeName = mxmlElementGetAttr(suffixNode, "name");
			
			cout << "node name = " << nodeName << endl;
			cout << "node type = " << nodeType << endl;
						
			if (nodeType == "STRING")
			{
				m_suffix = suffixNode->child->value.text.string;
			}
			
            auto pathNode = mxmlFindElement(suffixNode, node, "data", NULL, NULL, MXML_NO_DESCEND);
			nodeName = mxmlElementGetAttr(pathNode, "name");
            nodeType = mxmlElementGetAttr(pathNode, "type");

			cout << "node name = " << nodeName << endl;
			cout << "node type = " << nodeType << endl;

            if (nodeType == "STRING")
            {
                m_path = pathNode->child->value.text.string;
            }
            
        }
   }

    mxmlDelete(tree);
	
    DbgFuncExit();
}

string parseConfig::getSocketIp()
{
    return m_ip;
}


int parseConfig::getSocketPort()
{
	return m_port;
}

string parseConfig::getEventLogtPrefix()
{
    return m_prefix;
}

string parseConfig::getEventLogtSuffix()
{
	return m_suffix;
}

string parseConfig::getEventLogtPath()
{
	return m_path;
}



