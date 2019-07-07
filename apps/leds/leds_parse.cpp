#include		"common.h"
#include		"mxml.h"
#include		"leds_parse.h"

static leds_params_t leds_params;

void leds_parse_xml(const char *file)
{
	FILE *fp;
	mxml_node_t *tree, *root, *module, *channelItem, *baseValueNode;
	string item, name, type;
	int channel;

	DbgFuncEntry();
	
	DbgPrintf("XML configure file = %s\r\n", file);

	DbgFuncExit();
}

leds_params_t * get_sysmon_params(void)
{
	return &leds_params;
}

void leds_info_printf(void)
{
}
