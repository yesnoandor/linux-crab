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

	//DbgFuncEntry();
	
	//DbgPrintf("XML configure file = %s\r\n", file);

	fp = fopen(file, "r");
	tree = mxmlLoadFile(NULL, fp, MXML_TEXT_CALLBACK);
	fclose(fp);
	
	root = mxmlFindElement(tree, tree, "root", NULL, NULL, MXML_DESCEND);
	for(module = mxmlFindElement(root, tree, "module", NULL, NULL, MXML_DESCEND);module != NULL;module = mxmlFindElement(module, root, "module", NULL, NULL, MXML_DESCEND))
	{
		item = mxmlElementGetAttr(module, "name");
		//DbgPrintf("module name = %s \n", item.c_str());
		if (item == "sysmon")
		{
			baseValueNode = mxmlFindElement(module, module, "value", NULL, NULL, MXML_DESCEND);
			name = mxmlElementGetAttr(baseValueNode, "name");
			if (name == "node_base")
			{
				sysmon_params.node_base = baseValueNode->child->value.text.string;
			}
			
			for (channelItem = mxmlFindElement(baseValueNode, module, "item", NULL, NULL, MXML_NO_DESCEND); channelItem != NULL; channelItem = mxmlFindElement(channelItem, module, "item", NULL, NULL, MXML_NO_DESCEND))
			{
				channel = stoi(mxmlElementGetAttr(channelItem, "channel"), NULL, 10);
				type = mxmlElementGetAttr(channelItem, "type");
				if (type == "temp")
				{
					for (mxml_node_t *valueNode = mxmlFindElement(channelItem, channelItem, "value", NULL, NULL, MXML_DESCEND); valueNode != NULL; valueNode = mxmlFindElement(valueNode, channelItem, "value", NULL, NULL, MXML_NO_DESCEND))
					{
						if(channel<SYSTEM_TEMP_MAX_NODE)
						{
							name = mxmlElementGetAttr(valueNode, "name");
							if (name == "offset")
							{
								sysmon_params.temp_node[channel].offset  = valueNode->child->value.text.string;
								//DbgPrintf("offset = %s \r\n",sysmon_params.temp_node[channel].offset.c_str());
							}
							else if (name == "raw")
							{
								sysmon_params.temp_node[channel].raw = valueNode->child->value.text.string;
								//DbgPrintf("raw = %s \r\n",sysmon_params.temp_node[channel].raw.c_str());
							}
							else if (name == "scale")
							{
								sysmon_params.temp_node[channel].scale = valueNode->child->value.text.string;
								//DbgPrintf("scale = %s \r\n",sysmon_params.temp_node[channel].scale.c_str());
							}
							else if (name == "max")
							{
								sysmon_params.temp_node[channel].max_value = strtod((valueNode->child->value.text.string), NULL);
								//DbgPrintf("max = %lf \n", sysmon_params.temp_node[channel].max_value);
							}
							else if (name == "min")
							{
								sysmon_params.temp_node[channel].min_value = strtod((valueNode->child->value.text.string), NULL);
								//DbgPrintf("min = %lf \n", sysmon_params.temp_node[channel].min_value);
							}
						}
						else
						{
							DbgWarn("channel(%d) >= SYSTEM_TEMP_MAX_NODE(%d)\r\n",channel,SYSTEM_TEMP_MAX_NODE);
							continue;
						}
					}
				}
				else if(type == "voltage")
				{

					for (mxml_node_t *valueNode = mxmlFindElement(channelItem, channelItem, "value", NULL, NULL, MXML_DESCEND); valueNode != NULL; valueNode = mxmlFindElement(valueNode, channelItem, "value", NULL, NULL, MXML_NO_DESCEND))
					{
						if(channel < SYSTEM_VOLTAGE_MAX_NODE)
						{
							name = mxmlElementGetAttr(valueNode, "name");
							if (name == "raw")
							{
								sysmon_params.voltage_node[channel].raw = valueNode->child->value.text.string;
								//DbgPrintf("raw = %s \r\n",sysmon_params.voltage_node[channel].raw.c_str());
							}
							else if (name == "scale")
							{
								sysmon_params.voltage_node[channel].scale = valueNode->child->value.text.string;
								//DbgPrintf("scale = %s \r\n",sysmon_params.voltage_node[channel].scale.c_str());
							}
							else if (name == "max")
							{
								sysmon_params.voltage_node[channel].max_value = strtod((valueNode->child->value.text.string), NULL);
								//DbgPrintf("max = %lf \n", sysmon_params.voltage_node[channel].max_value);
							}
							else if (name == "min")
							{
								sysmon_params.voltage_node[channel].min_value = strtod((valueNode->child->value.text.string), NULL);
								//DbgPrintf("min = %lf \n", sysmon_params.voltage_node[channel].min_value);
							}
						}
						else
						{
							DbgWarn("channel(%d) >= SYSTEM_VOLTAGE_MAX_NODE(%d)\r\n",channel,SYSTEM_VOLTAGE_MAX_NODE);
							continue;
						}
					}
				}
				else
				{
					DbgWarn("sysmon type is not supported!(%s)\r\n",type.c_str());
					continue;
				}
			}
	
		}
		else if(item == "sysmon_pl")
		{
			baseValueNode = mxmlFindElement(module, module, "value", NULL, NULL, MXML_DESCEND);
			name = mxmlElementGetAttr(baseValueNode, "name");
			if (name == "node_base")
			{
				sysmon_params.node_base_pl = baseValueNode->child->value.text.string;
			}
			
			for (channelItem = mxmlFindElement(baseValueNode, module, "item", NULL, NULL, MXML_NO_DESCEND); channelItem != NULL; channelItem = mxmlFindElement(channelItem, module, "item", NULL, NULL, MXML_NO_DESCEND))
			{
				channel = stoi(mxmlElementGetAttr(channelItem, "channel"), NULL, 10);
				type = mxmlElementGetAttr(channelItem, "type");
				if (type == "temp")
				{
					for (mxml_node_t *valueNode = mxmlFindElement(channelItem, channelItem, "value", NULL, NULL, MXML_DESCEND); valueNode != NULL; valueNode = mxmlFindElement(valueNode, channelItem, "value", NULL, NULL, MXML_NO_DESCEND))
					{
						if(channel<SYSTEM_TEMP_MAX_NODE_PL)
						{
							name = mxmlElementGetAttr(valueNode, "name");
							if (name == "offset")
							{
								sysmon_params.temp_node_pl[channel].offset  = valueNode->child->value.text.string;
								//DbgPrintf("offset = %s \r\n",sysmon_params.temp_node_pl[channel].offset.c_str());
							}
							else if (name == "raw")
							{
								sysmon_params.temp_node_pl[channel].raw = valueNode->child->value.text.string;
								//DbgPrintf("raw = %s \r\n",sysmon_params.temp_node_pl[channel].raw.c_str());
							}
							else if (name == "scale")
							{
								sysmon_params.temp_node_pl[channel].scale = valueNode->child->value.text.string;
								//DbgPrintf("scale = %s \r\n",sysmon_params.temp_node_pl[channel].scale.c_str());
							}
							else if (name == "max")
							{
								sysmon_params.temp_node_pl[channel].max_value = strtod((valueNode->child->value.text.string), NULL);
								//DbgPrintf("max = %lf \n", sysmon_params.temp_node_pl[channel].max_value);
							}
							else if (name == "min")
							{
								sysmon_params.temp_node_pl[channel].min_value = strtod((valueNode->child->value.text.string), NULL);
								//DbgPrintf("min = %lf \n", sysmon_params.temp_node_pl[channel].min_value);
							}
						}
						else
						{
							DbgWarn("channel(%d) >= SYSTEM_TEMP_MAX_NODE_PL(%d)\r\n",channel,SYSTEM_TEMP_MAX_NODE_PL);
							continue;
						}
					}
				}
				else if(type == "voltage")
				{
					for (mxml_node_t *valueNode = mxmlFindElement(channelItem, channelItem, "value", NULL, NULL, MXML_DESCEND); valueNode != NULL; valueNode = mxmlFindElement(valueNode, channelItem, "value", NULL, NULL, MXML_NO_DESCEND))
					{
						if(channel < SYSTEM_VOLTAGE_MAX_NODE_PL)
						{
							name = mxmlElementGetAttr(valueNode, "name");
							if (name == "raw")
							{
								sysmon_params.voltage_node_pl[channel].raw = valueNode->child->value.text.string;
								//DbgPrintf("raw = %s \r\n",sysmon_params.voltage_node_pl[channel].raw.c_str());
							}
							else if (name == "scale")
							{
								sysmon_params.voltage_node_pl[channel].scale = valueNode->child->value.text.string;
								//DbgPrintf("scale = %s \r\n",sysmon_params.voltage_node_pl[channel].scale.c_str());
							}
							else if (name == "max")
							{
								sysmon_params.voltage_node_pl[channel].max_value = strtod((valueNode->child->value.text.string), NULL);
								//DbgPrintf("max = %lf \n", sysmon_params.voltage_node_pl[channel].max_value);
							}
							else if (name == "min")
							{
								sysmon_params.voltage_node_pl[channel].min_value = strtod((valueNode->child->value.text.string), NULL);
								//DbgPrintf("min = %lf \n", sysmon_params.voltage_node_pl[channel].min_value);
							}
						}
						else
						{
							DbgWarn("channel(%d) >= SYSTEM_VOLTAGE_MAX_NODE_PL(%d)\r\n",channel,SYSTEM_VOLTAGE_MAX_NODE_PL);
							continue;
						}
					}
				}
				else
				{
					DbgWarn("sysmon_pl type is not supported!(%s)\r\n",type.c_str());
					continue;
				}
			}
	
		}
		else
		{
			DbgError("test module(%s) is not %s!\r\n",item.c_str(),"sysmon or sysmon_pl");
			return;
		}
	}

	mxmlDelete(tree);

	//DbgFuncExit();
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
		DbgGood("++++++++++PS Temp Node(%d)++++++++++\r\n",i);
		DbgGood("offset = %s\r\n",sysmon_params.temp_node[i].offset.c_str());
		DbgGood("raw = %s\r\n",sysmon_params.temp_node[i].raw.c_str());
		DbgGood("scale = %s\r\n",sysmon_params.temp_node[i].scale.c_str());

		DbgGood("max = %lf\r\n",sysmon_params.temp_node[i].max_value);
		DbgGood("min = %lf\r\n",sysmon_params.temp_node[i].min_value);
	}

	for(i=0;i<SYSTEM_VOLTAGE_MAX_NODE;i++)
	{
		DbgGood("++++++++++PL Voltage Node(%d)++++++++++\r\n",i);
		DbgGood("raw = %s\r\n",sysmon_params.voltage_node[i].raw.c_str());
		DbgGood("scale = %s\r\n",sysmon_params.voltage_node[i].scale.c_str());

		DbgGood("max = %lf\r\n",sysmon_params.voltage_node[i].max_value);
		DbgGood("min = %lf\r\n",sysmon_params.voltage_node[i].min_value);
	}

	for(i=0;i<SYSTEM_TEMP_MAX_NODE_PL;i++)
	{
		DbgGood("++++++++++PL Temp Node(%d)++++++++++\r\n",i);
		DbgGood("offset = %s\r\n",sysmon_params.temp_node_pl[i].offset.c_str());
		DbgGood("raw = %s\r\n",sysmon_params.temp_node_pl[i].raw.c_str());
		DbgGood("scale = %s\r\n",sysmon_params.temp_node_pl[i].scale.c_str());

		DbgGood("max = %lf\r\n",sysmon_params.temp_node_pl[i].max_value);
		DbgGood("min = %lf\r\n",sysmon_params.temp_node_pl[i].min_value);
	}

	for(i=0;i<SYSTEM_VOLTAGE_MAX_NODE_PL;i++)
	{
		DbgGood("++++++++++PL Voltage Node(%d)++++++++++\r\n",i);
		DbgGood("raw = %s\r\n",sysmon_params.voltage_node_pl[i].raw.c_str());
		DbgGood("scale = %s\r\n",sysmon_params.voltage_node_pl[i].scale.c_str());

		DbgGood("max = %lf\r\n",sysmon_params.voltage_node_pl[i].max_value);
		DbgGood("min = %lf\r\n",sysmon_params.voltage_node_pl[i].min_value);
	}
}
