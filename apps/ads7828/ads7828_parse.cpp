#include		"common.h"
#include		"mxml.h"
#include		"ads7828_parse.h"

static ads7828_params_t ads7828_params;

void ads7828_parse_xml(const char *file)
{
	FILE *fp;
	mxml_node_t *tree, *root, *topItem, *channelItem, *baseValueNode;
	string item, name, enable;
	int channel;

	//DbgFuncEntry();
	
	//DbgPrintf("XML configure file = %s\r\n", file);

	fp = fopen(file, "r");
	tree = mxmlLoadFile(NULL, fp, MXML_TEXT_CALLBACK);
	fclose(fp);
	
	root = mxmlFindElement(tree, tree, "root", NULL, NULL, MXML_DESCEND);
	topItem = mxmlFindElement(root, tree, "item", NULL, NULL, MXML_DESCEND);
	item = mxmlElementGetAttr(topItem, "name");
	if (item != "ads7828")
	{
		DbgError("item mismatch!\r\n");
		return;
	}
	
	baseValueNode = mxmlFindElement(topItem, topItem, "value", NULL, NULL, MXML_DESCEND);
	name = mxmlElementGetAttr(baseValueNode, "name");
	if (name == "node_base")
	{
		ads7828_params.node_base = baseValueNode->child->value.text.string;
	}
	
	for (channelItem = mxmlFindElement(baseValueNode, topItem, "item", NULL, NULL, MXML_NO_DESCEND); channelItem != NULL; channelItem = mxmlFindElement(channelItem, topItem, "item", NULL, NULL, MXML_NO_DESCEND))
	{
		channel = stoi(mxmlElementGetAttr(channelItem, "channel"), NULL, 10);
		enable = mxmlElementGetAttr(channelItem, "enable");

		//DbgPrintf("channel = %d \r\n", channel);
		//DbgPrintf("enable = %s \r\n", enable.c_str());
		if (enable == "true")
		{
			ads7828_params.channel[channel].enable = 1;
		}
		else
		{
			ads7828_params.channel[channel].enable = 0;
			continue;
		}
		
		for (mxml_node_t *valueNode = mxmlFindElement(channelItem, channelItem, "value", NULL, NULL, MXML_DESCEND); valueNode != NULL; valueNode = mxmlFindElement(valueNode, channelItem, "value", NULL, NULL, MXML_NO_DESCEND))
		{
			name = mxmlElementGetAttr(valueNode, "name");
			if (name == "node")
			{
				ads7828_params.channel[channel].node.node_name = valueNode->child->value.text.string;
				//printf("nodeIndex = %d,name = %s \n", nodeIndex, ina3221_params.channel[channel].node[nodeIndex].node_name.c_str());
			}
			else if (name == "max")
			{
				ads7828_params.channel[channel].node.max_value = strtol((valueNode->child->value.text.string), NULL, 10);
				//printf("nodeIndex = %d,max = %ld \n", nodeIndex, ina3221_params.channel[channel].node[nodeIndex].max_value);
			}
			else if (name == "min")
			{
				ads7828_params.channel[channel].node.min_value = strtol((valueNode->child->value.text.string), NULL, 10);
				//printf("nodeIndex = %d ,min = %ld \n", nodeIndex, ina3221_params.channel[channel].node[nodeIndex].min_value);
			}
		}

		//DbgPrintf("node_name = %s\r\n",ads7828_params.channel[channel].node.node_name.c_str());
		//DbgPrintf("max_value = %d\r\n",ads7828_params.channel[channel].node.max_value);
		//DbgPrintf("min_value = %d\r\n",ads7828_params.channel[channel].node.min_value);
	
	}

	mxmlDelete(tree);

	//DbgFuncExit();
}

ads7828_params_t * get_ads7827_params(void)
{
	//DbgFuncEntry();

	//DbgFuncExit();
	
	return &ads7828_params;
}
