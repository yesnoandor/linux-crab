#include		"common.h"
#include		"memory_parse.h"

static memory_params_t memory_params;

void memory_parse_xml(const char *file)
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
	if (item != "memory")
	{
		DbgError("test module(%s) is not %s!\r\n",item.c_str(),"memory");
		return;
	}

	baseValueNode = mxmlFindElement(module, module, "value", NULL, NULL, MXML_DESCEND);
	name = mxmlElementGetAttr(baseValueNode, "name");
	if (name == "address")
	{
		memory_params.address = strtoll(baseValueNode->child->value.text.string,NULL,16);
		DbgPrintf("address = 0x%lx\r\n",memory_params.address);
	}

	baseValueNode = mxmlFindElement(baseValueNode, module, "value", NULL, NULL, MXML_NO_DESCEND);
	name = mxmlElementGetAttr(baseValueNode, "name");
	if (name == "size")
	{
		memory_params.size =  strtoll(baseValueNode->child->value.text.string,NULL,16);
		DbgPrintf("size = 0x%lx\r\n",memory_params.size);
	}

	
	mxmlDelete(tree);

	DbgFuncExit();
}

memory_params_t * get_memory_params(void)
{
	return &memory_params;
}

void memory_info_printf(void)
{
	int i;
	
	DbgFuncEntry();
	
	DbgGood("++++++++++Memory Info++++++++++\r\n",i);
	DbgGood("address = 0x%lx\r\n",memory_params.address);
	DbgGood("size = 0x%lx\r\n",memory_params.size);

	DbgFuncExit();
}
