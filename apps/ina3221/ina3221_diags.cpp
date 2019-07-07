#include		"common.h"
#include		"ina3221_parse.h"

#define		INA3221_XML		"./ina3221_diags.xml"



int sysfs_ina3221_test(void)
{
	int i,channel;
	
	int ret = ERROR_NO;
	string node;
	ina3221_params_t *params;
	int min,max,value;

	params = get_ina3221_params();
	
	for(channel = 0;channel < 3;channel++)
	{
		for(i=0;i<3;i++)
		{
			node = params->node_base + "/" + params->channel[channel].node[i].node_name;
			max = params->channel[channel].node[i].max_value;
			min = params->channel[channel].node[i].min_value;
		
			ret = get_node_item(node.c_str(),value);
			if(ret != ERROR_NO)
				return ret;

			DbgPrintf("value = 0x%x\r\n",value);
			DbgPrintf("max = 0x%x\r\n",max);
			DbgPrintf("min = 0x%x\r\n",min);

			if((value > max) || (value <  min))
			{
				ret = ERROR_MODULE_INA3221;
				DbgError("channel %d adc sample error!\r\n",channel);	
			}

		}

	}
	
	return ret;
}

int main(int argc,char* argv[])
{
	int ret;
	
	// parse xml
	ina3221_parse_xml("ina3221_diags.xml");

	ret = sysfs_ina3221_test();
	DbgPrintf("sysfs ina3221 test result = %d\r\n",ret);

	return ret;
}

