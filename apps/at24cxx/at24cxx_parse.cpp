#include	"common.h"
#include	"mxml.h"
#include	"at24cxx_parse.h"

static sysfs_params_t sysfs_params;
static devfs_params_t devfs_params;

void at24cxx_parse_xml(const char *file)
{
	FILE *fp;
	mxml_node_t *tree, *root, *node;
	string item, name;

	//DbgFuncEntry();

	//DbgPrintf("XML configure file = %s\r\n", file);

	fp = fopen(file, "r");
	tree = mxmlLoadFile(NULL, fp, MXML_TEXT_CALLBACK);
	fclose(fp);

	root = mxmlFindElement(tree, tree, "root", NULL, NULL, MXML_DESCEND);

	for (node = mxmlFindElement(root, tree, "item", NULL, NULL, MXML_DESCEND); node != NULL; node = mxmlFindElement(node, root, "item", NULL, NULL, MXML_NO_DESCEND))
	{
		item = mxmlElementGetAttr(node, "name");
		if (item == "sysfs")
		{
			string enable = mxmlElementGetAttr(node, "enable");
			if (enable == "true")
			{
				sysfs_params.enable = 1;
			}
			else
			{
				sysfs_params.enable = 0;
			}

			for (mxml_node_t * valueNode = mxmlFindElement(node, node, "value", NULL, NULL, MXML_DESCEND); valueNode != NULL; valueNode = mxmlFindElement(valueNode, node, "value", NULL, NULL, MXML_NO_DESCEND))
			{
				name = mxmlElementGetAttr(valueNode, "name");
				if (name == "node")
				{
					sysfs_params.node = valueNode->child->value.text.string;
				}
				else if (name == "offset")
				{
					sysfs_params.offset = strtol((valueNode->child->value.text.string), NULL, 16);
				}
				else if (name == "string")
				{
					sysfs_params.str = valueNode->child->value.text.string;
				}
			}
		}

		if (item == "devfs")
		{
			string enable = mxmlElementGetAttr(node, "enable");
			if (enable == "true")
			{
				devfs_params.enable = 1;
			}
			else
			{
				devfs_params.enable = 0;
			}

			for (mxml_node_t * valueNode = mxmlFindElement(node, node, "value", NULL, NULL, MXML_DESCEND); valueNode != NULL; valueNode = mxmlFindElement(valueNode, node, "value", NULL, NULL, MXML_NO_DESCEND))
			{
				name = mxmlElementGetAttr(valueNode, "name");
				if (name == "node")
				{
					devfs_params.node = valueNode->child->value.text.string;
				}
				else if (name == "slaveaddr")
				{
					devfs_params.slaveaddr = stol((valueNode->child->value.text.string), NULL, 16);
				}
                		else if (name == "offset")
                		{
					devfs_params.offset = strtol((valueNode->child->value.text.string), NULL, 16);
				}
				else if (name == "value")
				{
					devfs_params.value = strtol((valueNode->child->value.text.string), NULL, 16);
				}
			}
		}
	}

	mxmlDelete(tree);

	//DbgFuncExit();
}


sysfs_params_t * get_at24cxx_sysfs_params(void)
{
	return &sysfs_params;
}

devfs_params_t * get_at24cxx_devfs_params(void)
{
	return &devfs_params;
}

void at24cxx_info_printf(void)
{
	DbgGood("++++++++++Sysfs Node++++++++++\r\n");
	DbgGood("enable = %d\r\n",sysfs_params.enable);
	DbgGood("node = %s\r\n",sysfs_params.node.c_str());
	DbgGood("offset = %d\r\n",sysfs_params.offset);
	DbgGood("string = %s\r\n",sysfs_params.str.c_str());

	
	DbgGood("++++++++++Devfs Node++++++++++\r\n");
	DbgGood("enable = %d\r\n",devfs_params.enable);
	DbgGood("node = %s\r\n",devfs_params.node.c_str());
	DbgGood("slaveaddr = 0x%x\r\n",devfs_params.slaveaddr);
	DbgGood("offset = %d\r\n",devfs_params.offset);
	DbgGood("value = %d\r\n",devfs_params.value);
}

