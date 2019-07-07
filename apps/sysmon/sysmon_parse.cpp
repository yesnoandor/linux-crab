#include		"common.h"
#include		"mxml.h"
#include		"sysmon_parse.h"

static sysmon_params_t sysmon_params;

void sysmon_parse_xml(const char *file)
{
	FILE *fp;
	mxml_node_t *tree, *root, *module, *channelItem, *baseValueNode;
	string item, name, type;
	int channel;

	DbgFuncEntry();
	
	DbgPrintf("XML configure file = %s\r\n", file);

	fp = fopen(file, "r");
	tree = mxmlLoadFile(NULL, fp, MXML_TEXT_CALLBACK);
	fclose(fp);
	
	root = mxmlFindElement(tree, tree, "root", NULL, NULL, MXML_DESCEND);
	module = mxmlFindElement(root, tree, "module", NULL, NULL, MXML_DESCEND);
	item = mxmlElementGetAttr(module, "name");
	if (item != "sysmon")
	{
		DbgError("test module(%s) is not %s!\r\n",item.c_str(),"sysmon");
		return;
	}
	
	baseValueNode = mxmlFindElement(module, module, "value", NULL, NULL, MXML_DESCEND);
	name = mxmlElementGetAttr(baseValueNode, "name");
	if (name == "node_base")
	{
		sysmon_params.node_base = baseValueNode->child->value.text.string;
	}
	
	for (channelItem = mxmlFindElement(baseValueNode, module, "item", NULL, NULL, MXML_NO_DESCEND); channelItem != NULL; channelItem = mxmlFindElement(channelItem, module, "item", NULL, NULL, MXML_NO_DESCEND))
	{
		channel = stoi(mxmlElementGetAttr(channelItem, "channel"), NULL, 10);
		//printf("channel = %d \n", channel);
		type = mxmlElementGetAttr(channelItem, "type");
		//printf("enable = %s \n", enable.c_str());
		if (type == "temp")
		{
			for (mxml_node_t *valueNode = mxmlFindElement(channelItem, channelItem, "value", NULL, NULL, MXML_DESCEND); valueNode != NULL; valueNode = mxmlFindElement(valueNode, channelItem, "value", NULL, NULL, MXML_NO_DESCEND))
			{
				name = mxmlElementGetAttr(valueNode, "name");
				if (name == "offset")
				{
					sysmon_params.temp_node[channel].offset  = valueNode->child->value.text.string;
					DbgPrintf("offset = %s \r\n",sysmon_params.temp_node[channel].offset.c_str());
				}
				else if (name == "raw")
				{
					sysmon_params.temp_node[channel].raw = valueNode->child->value.text.string;
					DbgPrintf("raw = %s \r\n",sysmon_params.temp_node[channel].raw.c_str());
				}
				else if (name == "scale")
				{
					sysmon_params.temp_node[channel].scale = valueNode->child->value.text.string;
					DbgPrintf("scale = %s \r\n",sysmon_params.temp_node[channel].scale.c_str());
				}
				else if (name == "max")
				{
					sysmon_params.temp_node[channel].max_value = strtol((valueNode->child->value.text.string), NULL, 10);
					DbgPrintf("max = %ld \n", sysmon_params.temp_node[channel].max_value);
				}
				else if (name == "min")
				{
					sysmon_params.temp_node[channel].min_value = strtol((valueNode->child->value.text.string), NULL, 10);
					DbgPrintf("min = %ld \n", sysmon_params.temp_node[channel].min_value);
				}
			}
		}
		else if(type == "voltage")
		{
			for (mxml_node_t *valueNode = mxmlFindElement(channelItem, channelItem, "value", NULL, NULL, MXML_DESCEND); valueNode != NULL; valueNode = mxmlFindElement(valueNode, channelItem, "value", NULL, NULL, MXML_NO_DESCEND))
			{
				name = mxmlElementGetAttr(valueNode, "name");
				if (name == "raw")
				{
					sysmon_params.voltage_node[channel].raw = valueNode->child->value.text.string;
					DbgPrintf("raw = %s \r\n",sysmon_params.voltage_node[channel].raw.c_str());
				}
				else if (name == "scale")
				{
					sysmon_params.voltage_node[channel].scale = valueNode->child->value.text.string;
					DbgPrintf("scale = %s \r\n",sysmon_params.voltage_node[channel].scale.c_str());
				}
				else if (name == "max")
				{
					sysmon_params.voltage_node[channel].max_value = strtol((valueNode->child->value.text.string), NULL, 10);
					DbgPrintf("max = %ld \n", sysmon_params.temp_node[channel].max_value);
				}
				else if (name == "min")
				{
					sysmon_params.voltage_node[channel].min_value = strtol((valueNode->child->value.text.string), NULL, 10);
					DbgPrintf("min = %ld \n", sysmon_params.voltage_node[channel].min_value);
				}
			}
		}
		else
		{
			DbgWarn("sysmon type is not supported!(%s)\r\n",type.c_str());
			continue;
		}
	}
	
	mxmlDelete(tree);

	DbgFuncExit();
}

sysmon_params_t * get_sysmon_params(void)
{
	return &sysmon_params;
}

void sysmon_info_printf(void)
{
	int i;

	for(i=0;i<SYSTEM_TEMP_MAX_NODE;i++)
	{
		DbgGood("++++++++++Temp Node(%d)++++++++++\r\n",i);
		DbgGood("offset = %s\r\n",sysmon_params.temp_node[i].offset.c_str());
		DbgGood("raw = %s\r\n",sysmon_params.temp_node[i].raw.c_str());
		DbgGood("scale = %s\r\n",sysmon_params.temp_node[i].scale.c_str());

		DbgGood("max = %d\r\n",sysmon_params.temp_node[i].max_value);
		DbgGood("min = %d\r\n",sysmon_params.temp_node[i].min_value);
	}

	for(i=0;i<SYSTEM_VOLTAGE_MAX_NODE;i++)
	{
		DbgGood("++++++++++Voltage Node(%d)++++++++++\r\n",i);
		DbgGood("raw = %s\r\n",sysmon_params.voltage_node[i].raw.c_str());
		DbgGood("scale = %s\r\n",sysmon_params.voltage_node[i].scale.c_str());

		DbgGood("max = %d\r\n",sysmon_params.voltage_node[i].max_value);
		DbgGood("min = %d\r\n",sysmon_params.voltage_node[i].min_value);
	}
}
