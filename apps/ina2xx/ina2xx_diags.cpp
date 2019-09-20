#include		"common.h"
#include		"ina2xx_parse.h"

#define		INA2XX_XML		"./ina2xx_diags.xml"

int sysfs_ina2xx_test()
{
	int i;
	int ret = ERROR_NO;
	
	string node;
	ina2xx_params_t *params;
	int min,max,value;

	DbgFuncEntry();

	params = get_ina2xx_params();

	for(i=0;i<INA2XX_MAX_NODE;i++)
	{
		node = params->node_base + "/" + params->node[i].node_name;
		max = params->node[i].max_value;
		min = params->node[i].min_value;

		
		DbgPrintf("node = %s\r\n",node.c_str());
		
		ret = get_node_item(node.c_str(),value);
		if(ret != ERROR_NO)
			return ret;

		DbgPrintf("value = %d\r\n",value);
		DbgPrintf("max = %d\r\n",max);
		DbgPrintf("min = %d\r\n",min);

		if((value > max) || (value <  min))
		{
			ret = -ERROR_MODULE_INA2xx;
			DbgError("channel %d adc sample error!\r\n");	
			break;
		}

		DbgGood("read ok!\r\n");
	}

	DbgFuncExit();
	return ret;
}



int main(int argc,char* argv[])
{
	int ret;

	// parse xml
	ina2xx_parse_xml(INA2XX_XML);
	ina2xx_info_printf();

	ret = sysfs_ina2xx_test();
	DbgPrintf("sysfs ina2xx test result = %d\r\n",ret);

	return ret;
}
