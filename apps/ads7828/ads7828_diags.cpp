#include		"common.h"
#include		"ads7828_parse.h"

#define		ADS7828_XML		"./ads7828_diags.xml"

int sysfs_ads7828_test(void)
{
	int i;
	int ret = ERROR_NO;
	ads7828_params_t * params;
	string node;
	int value,max,min;

	params = get_ads7827_params();
	
	for(i=0;i<ADS7828_MAX_CHANNEL;i++)
	{
		node = params->node_base + "/" + params->channel[i].node.node_name;
		max = params->channel[i].node.max_value;
		min = params->channel[i].node.min_value;
		
		ret = get_node_item(node.c_str(),value);
		if(ret != ERROR_NO)
			return ret;

		DbgPrintf("max = 0x%x,\r\n",max);
		DbgPrintf("min = 0x%x,\r\n",min);
		DbgPrintf("value = 0x%x,\r\n",value);
		
		if((value < min) || (value > max))
		{
			ret = ERROR_MODULE_ADS7828;
			DbgError("channel %d adc sample error!\r\n",i);
		}
	}

	return ret;
}

int main(int argc,char* argv[])
{
	int ret;

	// parse ads7828 xml
	ads7828_parse_xml(ADS7828_XML);

	ret = sysfs_ads7828_test();
	DbgPrintf("sysfs ads7828 test result = %d\r\n",ret);

	return ret;
}

