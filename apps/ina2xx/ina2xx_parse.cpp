#include		"common.h"
#include		"mxml.h"
#include		"ina2xx_parse.h"

static ina2xx_params_t ina2xx_params;


void ina2xx_parse_xml(const char *file)
{
	FILE *fp;
	mxml_node_t *tree, *root, *module, *baseValueNode,*nodeItem;
	string item, name, enable;
	int channel, nodeIndex;

	DbgFuncEntry();
	DbgPrintf("XML configure file = %s\r\n", file);

	fp = fopen(file, "r");
	tree = mxmlLoadFile(NULL, fp, MXML_TEXT_CALLBACK);
	fclose(fp);

	root = mxmlFindElement(tree, tree, "root", NULL, NULL, MXML_DESCEND);
	module = mxmlFindElement(root, tree, "module", NULL, NULL, MXML_DESCEND);
	item = mxmlElementGetAttr(module, "name");
	if (item != "ina2xx")
	{
		DbgError("test module(%s) is not %s!\r\n",item.c_str(),"sysmon");
		return;
	}
	
	baseValueNode = mxmlFindElement(module, module, "value", NULL, NULL, MXML_DESCEND);
	name = mxmlElementGetAttr(baseValueNode, "name");
	if (name == "node_base")
	{
		ina2xx_params.node_base = baseValueNode->child->value.text.string;
		DbgPrintf("node_base = %s\r\n",ina2xx_params.node_base.c_str());
	}

	for (nodeItem = mxmlFindElement(baseValueNode, module, "item", NULL, NULL, MXML_NO_DESCEND); nodeItem != NULL; nodeItem = mxmlFindElement(nodeItem, module, "item", NULL, NULL, MXML_NO_DESCEND))
	{
		nodeIndex = stoi(mxmlElementGetAttr(nodeItem, "index"), NULL, 10);
		DbgPrintf("nodeIndex222 = %d\r\n", nodeIndex);

		for (mxml_node_t *valueNode = mxmlFindElement(nodeItem, nodeItem, "value", NULL, NULL, MXML_DESCEND); valueNode != NULL; valueNode = mxmlFindElement(valueNode, nodeItem, "value", NULL, NULL, MXML_NO_DESCEND))
		{
			name = mxmlElementGetAttr(valueNode, "name");
			if (name == "node")
			{
				ina2xx_params.node[nodeIndex].node_name = valueNode->child->value.text.string;	
				DbgPrintf("name111 = %s\r\n",ina2xx_params.node[nodeIndex].node_name.c_str());
			}
			else if (name == "max")
			{
				ina2xx_params.node[nodeIndex].max_value = strtol((valueNode->child->value.text.string), NULL, 10);
				DbgPrintf("max = %d\r\n",ina2xx_params.node[nodeIndex].max_value);
			}
			else if (name == "min")
			{
				ina2xx_params.node[nodeIndex].min_value = strtol((valueNode->child->value.text.string), NULL, 10);
				DbgPrintf("min22 = %d\r\n",ina2xx_params.node[nodeIndex].min_value);
			}
		}
	}

	mxmlDelete(tree);

	DbgFuncExit();
}

ina2xx_params_t * get_ina2xx_params(void)
{
	return &ina2xx_params;
}

void ina2xx_info_printf(void)
{
	int i;
	
	DbgFuncEntry();
	
	DbgGood("++++++++++INA2xx Info++++++++++\r\n",i);
	DbgGood("enable = %d\r\n",ina2xx_params.enable);
	DbgGood("node_base = %s\r\n",ina2xx_params.node_base.c_str());
	
	for(i=0;i<INA2XX_MAX_NODE;i++)
	{
		DbgGood("%d node-----\r\n",i);
		DbgGood("node name = %s\r\n",ina2xx_params.node[i].node_name.c_str());
		DbgGood("max = %d\r\n",ina2xx_params.node[i].max_value);
		DbgGood("min = %d\r\n",ina2xx_params.node[i].min_value);
	}

	DbgFuncExit();
}
