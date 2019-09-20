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

	DbgFuncEntry();

	params = get_ina3221_params();
	
	for(channel = 0;channel < INA3221_MAX_CHANNEL;channel++)
	{
		DbgPrintf("enable = %d\r\n",params->channel[channel].enable);
		
		if(params->channel[channel].enable)
		{
			for(i=0;i<3;i++)
			{
				node = params->node_base + "/" + params->channel[channel].node[i].node_name;
				max = params->channel[channel].node[i].max_value;
				min = params->channel[channel].node[i].min_value;
			
				ret = get_node_item(node.c_str(),value);
				if(ret != ERROR_NO)
					return ret;

				DbgPrintf("value = %d\r\n",value);
				DbgPrintf("max = %d\r\n",max);
				DbgPrintf("min = %d\r\n",min);

				if((value > max) || (value <  min))
				{
					ret = ERROR_MODULE_INA3221;
					DbgError("channel %d node (%d) monitor error!\r\n",channel,i);	
				}
				else
				{
					DbgGood("channel %d node (%d) monitor sample OK!\r\n",channel,i);	
				}
			}
		}
	}

	DbgFuncExit();

	return ret;
}

int main(int argc,char* argv[])
{
	int ret;
	
	// parse xml
	ina3221_parse_xml(INA3221_XML);

	ret = sysfs_ina3221_test();
	//DbgPrintf("sysfs ina3221 test result = %d\r\n",ret);

	if(!ret)
	{
		DbgGood("\r\n\r\n");
		DbgGood("=========================\r\n");
		DbgGood("=========================\r\n");
		DbgGood("sysfs ina3221 test ok!!!\r\n");
		DbgGood("=========================\r\n");
		DbgGood("=========================\r\n");
		DbgGood("\r\n\r\n");
	}
	else
	{
		DbgError("\r\n\r\n");
		DbgError("=========================\r\n");
		DbgError("=========================\r\n");
		DbgError("sysfs ina3221 test fail!!!\r\n");
		DbgError("=========================\r\n");
		DbgError("=========================\r\n");
		DbgError("\r\n\r\n");
	}

	return ret;
}

