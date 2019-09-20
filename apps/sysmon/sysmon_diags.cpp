#include		"common.h"
#include		"sysmon_parse.h"


#define		SYSMON_XML		"./sysmon_diags.xml"


int sysfs_sysmon_test(void)
{
	int i;
	int ret = ERROR_NO;
	sysmon_params_t * params;

	string node_offset,node_raw,node_scale;
	int value_offset,value_raw;
	double Celsius,voltage,value_scale;
	double max,min;
	
	
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

		//ret = get_node_item_sysmon(node_scale.c_str(),value_scale);
		ret = get_node_item(node_scale.c_str(),value_scale);
		if(ret != ERROR_NO)
			return ret;

		Celsius = ((value_raw + value_offset) * value_scale / 1000);

		//DbgGood("value_offset = %d\r\n",value_offset);
		//DbgGood("value_raw = %d\r\n",value_raw);
		//DbgGood("value_scale = %lf\r\n",value_scale);
		//DbgGood("Celsius = %lf\r\n",Celsius);
		
		max = params->temp_node[i].max_value;
		min = params->temp_node[i].min_value;

		if((Celsius < min) || (Celsius > max))
		{
			ret = -ERROR_MODULE_SYSMON;
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

		//ret = get_node_item_sysmon(node_scale.c_str(),value_scale);
		ret = get_node_item(node_scale.c_str(),value_scale);
		if(ret != ERROR_NO)
			return ret;

		voltage = (value_raw * value_scale)/1000;
		
		//DbgGood("node_raw = %s\r\n",node_raw.c_str());
		//DbgGood("node_scale = %s\r\n",node_scale.c_str());
		//DbgGood("value_raw = %d\r\n",value_raw);
		//DbgGood("value_scale = %lf\r\n",value_scale);
		//DbgGood("voltage = %lf\r\n",voltage);
		
		max = params->voltage_node[i].max_value;
		min = params->voltage_node[i].min_value;

		if((voltage < min) || (voltage > max))
		{
			ret = -ERROR_MODULE_SYSMON;
			DbgError("sysmon voltage error!\r\n");
		}
	}

	for(i=0;i<SYSTEM_TEMP_MAX_NODE_PL;i++)
	{
		node_offset = params->node_base_pl + "/" + params->temp_node_pl[i].offset;
		node_raw = params->node_base_pl + "/" + params->temp_node_pl[i].raw;
		node_scale = params->node_base_pl + "/" + params->temp_node_pl[i].scale;

		ret = get_node_item(node_offset.c_str(),value_offset);
		if(ret != ERROR_NO)
			return ret;

		ret = get_node_item(node_raw.c_str(),value_raw);
		if(ret != ERROR_NO)
			return ret;

		//ret = get_node_item_sysmon(node_scale.c_str(),value_scale);
		ret = get_node_item(node_scale.c_str(),value_scale);
		if(ret != ERROR_NO)
			return ret;

		Celsius = ((value_raw + value_offset) * value_scale / 1000);

		//DbgGood("pl value_offset = %d\r\n",value_offset);
		//DbgGood("pl value_raw = %d\r\n",value_raw);
		//DbgGood("pl value_scale = %lf\r\n",value_scale);
		//DbgGood("pl Celsius = %lf\r\n",Celsius);
		
		max = params->temp_node_pl[i].max_value;
		min = params->temp_node_pl[i].min_value;

		if((Celsius < min) || (Celsius > max))
		{
			ret = -ERROR_MODULE_SYSMON;
			DbgError("sysmon Celsius error!\r\n");
		}
	}

	for(i=0;i<SYSTEM_VOLTAGE_MAX_NODE_PL;i++)
	{
		node_raw = params->node_base_pl + "/" + params->voltage_node_pl[i].raw;
		node_scale = params->node_base_pl + "/" + params->voltage_node_pl[i].scale;

		ret = get_node_item(node_raw.c_str(),value_raw);
		if(ret != ERROR_NO)
			return ret;

		//ret = get_node_item_sysmon(node_scale.c_str(),value_scale);
		ret = get_node_item(node_scale.c_str(),value_scale);
		if(ret != ERROR_NO)
			return ret;

		voltage = (value_raw * value_scale)/1000;

		//DbgGood("pl node_raw = %s\r\n",node_raw.c_str());
		//DbgGood("pl node_scale = %s\r\n",node_scale.c_str());
		//DbgGood("pl value_raw = %d\r\n",value_raw);
		//DbgGood("pl value_scale = %lf\r\n",value_scale);
		//DbgGood("pl voltage = %lf\r\n",voltage);
		
		max = params->voltage_node_pl[i].max_value;
		min = params->voltage_node_pl[i].min_value;

		if((voltage < min) || (voltage > max))
		{
			ret = -ERROR_MODULE_SYSMON;
			DbgError("sysmon pl voltage error!\r\n");
		}
	}
	
	return ret;
}

int main(int argc,char* argv[])
{
	int ret;

	// parse sysmon xml
	sysmon_parse_xml(SYSMON_XML);
	//sysmon_info_printf();
	
	ret = sysfs_sysmon_test();
	//DbgPrintf("sysmon test result = %d\r\n",ret);

	if(!ret)
	{
		DbgGood("\r\n\r\n");
		DbgGood("=========================\r\n");
		DbgGood("=========================\r\n");
		DbgGood("sysmon test ok!!!\r\n");
		DbgGood("=========================\r\n");
		DbgGood("=========================\r\n");
		DbgGood("\r\n\r\n");
	}
	else
	{
		DbgError("\r\n\r\n");
		DbgError("=========================\r\n");
		DbgError("=========================\r\n");
		DbgError("sysmon test fail!!!\r\n");
		DbgError("=========================\r\n");
		DbgError("=========================\r\n");
		DbgError("\r\n\r\n");
	}

	return ret;
}


