#ifndef INA3221_PARSE_H_
#define INA3221_PARSE_H_

#define		INA3221_MAX_CHANNEL		3

//#ifdef __cplusplus
//	extern "C" {
//#endif

typedef struct{
	string node_name;
	int min_value;
	int max_value;
}ina3221_node_params_t,*p_ina3221_node_params_t;

typedef struct{
	int enable;
	ina3221_node_params_t node[3];
}ina3221_channel_params_t,*p_ina3221_channel_params_t;

typedef struct{
	string node_base;
	ina3221_channel_params_t channel[3];
}ina3221_params_t,*p_ina3221_params_t;


void ina3221_parse_xml(const char *file);
ina3221_params_t *get_ina3221_params(void);


//#ifdef __cplusplus
//	}
//#endif



#endif

