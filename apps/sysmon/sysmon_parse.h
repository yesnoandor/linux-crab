#ifndef	_SYSMON_PARSE_H_
#define	_SYSMON_PARSE_H_

//#ifdef __cplusplus
//	extern "C" {
//#endif

#define		SYSTEM_TEMP_MAX_NODE	3
#define		SYSTEM_VOLTAGE_MAX_NODE	2


typedef struct{
	string offset;
	string raw;
	string scale;
	int max_value;
	int min_value;
}sysmon_temp_node_params_t,*p_sysmon_temp_node_params_t;

typedef struct{
	string raw;
	string scale;
	int max_value;
	int min_value;
}sysmon_voltage_node_params_t,*p_voltage_temp_node_params_t;

typedef struct{
	string node_base;

	sysmon_temp_node_params_t temp_node[SYSTEM_TEMP_MAX_NODE];
	sysmon_voltage_node_params_t voltage_node[SYSTEM_VOLTAGE_MAX_NODE];
	
	//sysmon_temp_node_params_t ps_temp;
	//sysmon_temp_node_params_t remote_temp;
	//sysmon_temp_node_params_t pl_temp;

	//sysmon_voltage_node_params_t vcc_pspll0;
	//sysmon_voltage_node_params_t vcc_psbatt;
}sysmon_params_t,*p_sysmon_params_t;

void sysmon_parse_xml(const char *file);
sysmon_params_t * get_sysmon_params(void);
void sysmon_info_printf(void);

//#ifdef __cplusplus
//	}
//#endif



#endif

