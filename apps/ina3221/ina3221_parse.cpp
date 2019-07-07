#include "common.h"
#include "mxml.h"
#include "ina3221_parse.h"

static ina3221_params_t ina3221_params;

void ina3221_parse_xml(const char *file)
{
	FILE *fp;
	mxml_node_t *tree, *root, *topItem, *channelItem, *baseValueNode;
	string item, name, enable;
	int channel, nodeIndex;

	DbgFuncEntry();
	DbgPrintf("XML configure file = %s\r\n", file);

	fp = fopen(file, "r");
	tree = mxmlLoadFile(NULL, fp, MXML_TEXT_CALLBACK);
	fclose(fp);
	root = mxmlFindElement(tree, tree, "root", NULL, NULL, MXML_DESCEND);
	topItem = mxmlFindElement(root, tree, "item", NULL, NULL, MXML_DESCEND);
	item = mxmlElementGetAttr(topItem, "name");
	if (item != "ina3221")
	{
		return;
	}
	baseValueNode = mxmlFindElement(topItem, topItem, "value", NULL, NULL, MXML_DESCEND);
	name = mxmlElementGetAttr(baseValueNode, "name");
	if (name == "node_base")
	{
		ina3221_params.node_base = baseValueNode->child->value.text.string;
	}
	for (channelItem = mxmlFindElement(baseValueNode, topItem, "item", NULL, NULL, MXML_NO_DESCEND); channelItem != NULL; channelItem = mxmlFindElement(channelItem, topItem, "item", NULL, NULL, MXML_NO_DESCEND))
	{
		channel = stoi(mxmlElementGetAttr(channelItem, "channel"), NULL, 10);
		//printf("channel = %d \n", channel);
		enable = mxmlElementGetAttr(channelItem, "enable");
		//printf("enable = %s \n", enable.c_str());
		if (enable == "true")
		{
			ina3221_params.channel[channel].enable = 1;
			nodeIndex = -1;
		}
		else
		{
			ina3221_params.channel[channel].enable = 0;
			continue;
		}
		for (mxml_node_t *valueNode = mxmlFindElement(channelItem, channelItem, "value", NULL, NULL, MXML_DESCEND); valueNode != NULL; valueNode = mxmlFindElement(valueNode, channelItem, "value", NULL, NULL, MXML_NO_DESCEND))
		{
			name = mxmlElementGetAttr(valueNode, "name");
			if (name == "node")
			{
				nodeIndex++;
				ina3221_params.channel[channel].node[nodeIndex].node_name = valueNode->child->value.text.string;
				//printf("nodeIndex = %d,name = %s \n", nodeIndex, ina3221_params.channel[channel].node[nodeIndex].node_name.c_str());
			}
			else if (name == "max")
			{
				ina3221_params.channel[channel].node[nodeIndex].max_value = strtol((valueNode->child->value.text.string), NULL, 10);
				//printf("nodeIndex = %d,max = %ld \n", nodeIndex, ina3221_params.channel[channel].node[nodeIndex].max_value);
			}
			else if (name == "min")
			{
				ina3221_params.channel[channel].node[nodeIndex].min_value = strtol((valueNode->child->value.text.string), NULL, 10);
				//printf("nodeIndex = %d ,min = %ld \n", nodeIndex, ina3221_params.channel[channel].node[nodeIndex].min_value);
			}
		}
	}

	mxmlDelete(tree);

	DbgFuncExit();
}

ina3221_params_t *get_ina3221_params(void)
{
	return &ina3221_params;
}