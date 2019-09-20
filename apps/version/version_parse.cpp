#include	"common.h"
#include	"version_parse.h"

static version_params_t version_params;

void version_parse_xml(const char *file)
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
		if (item == "hw_version")
		{
			string enable = mxmlElementGetAttr(node, "enable");
			if (enable == "true")
			{
				version_params.hw_enable = 1;
			}
			else
			{
				version_params.hw_enable = 0;
			}

			for (mxml_node_t * valueNode = mxmlFindElement(node, node, "value", NULL, NULL, MXML_DESCEND); valueNode != NULL; valueNode = mxmlFindElement(valueNode, node, "value", NULL, NULL, MXML_NO_DESCEND))
			{
				name = mxmlElementGetAttr(valueNode, "name");
				if (name == "major")
				{
					version_params.hw_major_node = valueNode->child->value.text.string;
				}
				name = mxmlElementGetAttr(valueNode, "name");
				if (name == "minor")
				{
					version_params.hw_minor_node =  valueNode->child->value.text.string;
				}
			}
		}

		if (item == "sw_version")
		{
			string enable = mxmlElementGetAttr(node, "enable");
			if (enable == "true")
			{
				version_params.sw_enable = 1;
			}
			else
			{
				version_params.sw_enable = 0;
			}

			for (mxml_node_t * valueNode = mxmlFindElement(node, node, "value", NULL, NULL, MXML_DESCEND); valueNode != NULL; valueNode = mxmlFindElement(valueNode, node, "value", NULL, NULL, MXML_NO_DESCEND))
			{
				name = mxmlElementGetAttr(valueNode, "name");
				if (name == "version")
				{
					version_params.sw_version = valueNode->child->value.text.string;
				}
			}
		}
	}

	mxmlDelete(tree);

	//DbgFuncExit();	
}


version_params_t * get_version_params(void)
{
	return &version_params;
}


void version_info_printf(void)
{
	DbgGood("++++++++++Version++++++++++\r\n");
	DbgGood("hw enable = %d\r\n",version_params.hw_enable);
	DbgGood("hw major node = %s\r\n",version_params.hw_major_node.c_str());
	DbgGood("hw minor node = %s\r\n",version_params.hw_minor_node.c_str());
	DbgGood("sw enable = %d\r\n",version_params.sw_enable);
	DbgGood("version = %s\r\n",version_params.sw_version.c_str());
}

