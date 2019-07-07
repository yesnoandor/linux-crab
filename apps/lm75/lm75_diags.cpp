#include		"common.h"
#include		"lm75_parse.h"

#define		LM75_XML		"./lm75_diags.xml"

int sysfs_lm75_test()
{
	int i;
	int ret = ERROR_NO;

	string node;
	lm75_params_t *params;
	int min,max,value;

	node = params->node_base + "/" + params->node.name;
	max = params->node.max_value;
	min = params->node.min_value;
	
	ret = get_node_item(node.c_str(),value);
	if(ret != ERROR_NO)
		return ret;

	DbgPrintf("value = %d\r\n",value);
	DbgPrintf("max = %d\r\n",max);
	DbgPrintf("min = %d\r\n",min);

	if((value > max) || (value <  min))
	{
		ret = ERROR_MODULE_TMP75;
		DbgError("temperature error!\r\n");	
	}

	return ret;
}


int main(int argc,char* argv[])
{
	int ret;

	// parse xml
	lm75_parse_xml(LM75_XML);
	lm75_info_printf();

	ret = sysfs_lm75_test();
	DbgPrintf("sysfs tmp75-Q1 test result = %d\r\n",ret);

	return ret;
}

