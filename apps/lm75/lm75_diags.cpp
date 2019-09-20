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

	//DbgFuncEntry();

	params = get_lm75_params();

	node = params->node_base + "/" + params->node.name;
	max = params->node.max_value;
	min = params->node.min_value;

	//DbgPrintf("node = %s\r\n",node.c_str());
	
	ret = get_node_item(node.c_str(),value);
	if(ret != ERROR_NO)
		return ret;

	//DbgPrintf("value = %d\r\n",value);
	//DbgPrintf("max = %d\r\n",max);
	//DbgPrintf("min = %d\r\n",min);

	if((value > max) || (value <  min))
	{
		ret = ERROR_MODULE_TMP75;
		//DbgError("temperature error!\r\n");	
	}
	else
	{
		//DbgGood("temperature OK!\r\n");
	}

	//DbgFuncExit();
	
	return ret;
}


int main(int argc,char* argv[])
{
	int ret;

	// parse xml
	lm75_parse_xml(LM75_XML);
	//lm75_info_printf();

	ret = sysfs_lm75_test();
	//DbgPrintf("sysfs tmp75-Q1 test result = %d\r\n",ret);
	
	if(!ret)
	{
		DbgGood("\r\n\r\n");
		DbgGood("=========================\r\n");
		DbgGood("=========================\r\n");
		DbgGood("sysfs tmp75-Q1 test ok!!!\r\n");
		DbgGood("=========================\r\n");
		DbgGood("=========================\r\n");
		DbgGood("\r\n\r\n");
	}
	else
	{
		DbgError("\r\n\r\n");
		DbgError("=========================\r\n");
		DbgError("=========================\r\n");
		DbgError("sysfs tmp75-Q1 test fail!!!\r\n");
		DbgError("=========================\r\n");
		DbgError("=========================\r\n");
		DbgError("\r\n\r\n");
	}

	return ret;
}

