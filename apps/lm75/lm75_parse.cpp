#include		"common.h"
#include		"mxml.h"
#include		"lm75_parse.h"

static lm75_params_t lm75_params;

void lm75_parse_xml(const char *file)
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
	module = mxmlFindElement(root, tree, "module", NULL, NULL, MXML_DESCEND);
	item = mxmlElementGetAttr(module, "name");
	if (item != "lm75")
	{
		DbgError("test module(%s) is not %s!\r\n",item.c_str(),"lm75");
		return;
	}

	baseValueNode = mxmlFindElement(module, module, "value", NULL, NULL, MXML_DESCEND);
	name = mxmlElementGetAttr(baseValueNode, "name");
	if (name == "node_base")
	{
		lm75_params.node_base = baseValueNode->child->value.text.string;
	}

	for (mxml_node_t *valueNode = mxmlFindElement(baseValueNode, module, "value", NULL, NULL, MXML_DESCEND); valueNode != NULL; valueNode = mxmlFindElement(valueNode, module, "value", NULL, NULL, MXML_NO_DESCEND))
	{
		name = mxmlElementGetAttr(valueNode, "name");
		if (name == "node")
		{
			lm75_params.node.name  = valueNode->child->value.text.string;
			//DbgPrintf("name = %s \r\n",lm75_params.node.name.c_str());
		}
		else if (name == "max")
		{
			lm75_params.node.max_value = stoi((valueNode->child->value.text.string), NULL, 10);
			//DbgPrintf("max = %d \r\n", lm75_params.node.max_value);
		}
		else if (name == "min")
		{
			lm75_params.node.min_value = stoi((valueNode->child->value.text.string), NULL, 10);
			//DbgPrintf("min = %d \n",lm75_params.node.min_value);
		}
	}

	mxmlDelete(tree);

	//DbgFuncExit();
}


lm75_params_t * get_lm75_params(void)
{
	return &lm75_params;
}

void lm75_info_printf(void)
{
	DbgPrintf("++++++++++LM76 Info++++++++++\r\n");
	DbgPrintf("node_base = %s\r\n",lm75_params.node_base.c_str());
	DbgPrintf("name = %s\r\n",lm75_params.node.name.c_str());

	DbgPrintf("max = %d\r\n",lm75_params.node.max_value);
	DbgPrintf("min = %d\r\n",lm75_params.node.min_value);
}
