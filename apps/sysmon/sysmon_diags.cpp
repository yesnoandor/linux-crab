#include		"common.h"
#include		"sysmon_parse.h"


#define		SYSMON_XML		"./sysmon_diags.xml"

int sysfs_sysmon_test(void)
{
	int i;
	int ret = ERROR_NO;
	sysmon_params_t * params;

	string node_offset,node_raw,node_scale;
	int value_offset,value_raw,value_scale;
	int Celsius,voltage;
	int max,min;
	
	params = get_sysmon_params();

	for(i=0;i<SYSTEM_TEMP_MAX_NODE;i++)
	{
		node_offset = params->node_base + "/" + params->temp_node[i].offset;
		node_raw = params->node_base + "/" + params->temp_node[i].raw;
		node_scale = params->node_base + "/" + params->temp_node[i].scale;

		ret = get_node_item(node_offset.c_str(),value_offset);
		if(ret != ERROR_NO)
			return ret;

		ret = get_node_item(node_raw.c_str(),value_raw);
		if(ret != ERROR_NO)
			return ret;

		ret = get_node_item(node_scale.c_str(),value_scale);
		if(ret != ERROR_NO)
			return ret;

		Celsius = (value_raw + value_offset) * value_scale;

		DbgGood("value_offset = %d\r\n",value_offset);
		DbgGood("value_raw = %d\r\n",value_raw);
		DbgGood("value_scale = %d\r\n",value_scale);
		DbgGood("Celsius = %d\r\n",Celsius);
		
		max = params->temp_node[i].max_value;
		min = params->temp_node[i].min_value;

		if((Celsius < min) || (Celsius > max))
		{
			ret = ERROR_MODULE_SYSMON;
			DbgError("sysmon Celsius error!\r\n");
		}
	}

	for(i=0;i<SYSTEM_VOLTAGE_MAX_NODE;i++)
	{
		node_raw = params->node_base + "/" + params->voltage_node[i].raw;
		node_scale = params->node_base + "/" + params->voltage_node[i].scale;

		ret = get_node_item(node_raw.c_str(),value_raw);
		if(ret != ERROR_NO)
			return ret;

		ret = get_node_item(node_scale.c_str(),value_scale);
		if(ret != ERROR_NO)
			return ret;

		voltage = value_raw * value_scale;

		DbgGood("value_raw = %d\r\n",value_raw);
		DbgGood("value_scale = %d\r\n",value_scale);
		DbgGood("voltage = %d\r\n",voltage);
		
		max = params->voltage_node[i].max_value;
		min = params->voltage_node[i].min_value;

		if((voltage < min) || (voltage > max))
		{
			ret = ERROR_MODULE_SYSMON;
			DbgError("sysmon voltage error!\r\n");
		}
	}
	
	return ret;
}

int main(int argc,char* argv[])
{
	int ret;

	// parse sysmon xml
	sysmon_parse_xml(SYSMON_XML);
	sysmon_info_printf();
	
	ret = sysfs_sysmon_test();
	DbgPrintf("sysmon test result = %d\r\n",ret);

	return ret;
}


