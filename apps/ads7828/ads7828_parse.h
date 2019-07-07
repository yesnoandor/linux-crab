#ifndef	_ADS7828_PARSE_H_
#define	_ADS7828_PARSE_H_

//#ifdef __cplusplus
//	extern "C" {
//#endif

#define		ADS7828_MAX_CHANNEL		8

typedef struct{
	string node_name;
	int min_value;
	int max_value;
}ads7828_node_params_t,*p_ads7828_node_params_t;

typedef struct{
	int enable;
	ads7828_node_params_t node;
}ads7828_channel_params_t,*p_ads7828_channel_params_t;

typedef struct{
	string node_base;
	ads7828_channel_params_t channel[ADS7828_MAX_CHANNEL];
}ads7828_params_t,*p_ads7828_params_t;

void ads7828_parse_xml(const char *file);
ads7828_params_t * get_ads7827_params(void);

//#ifdef __cplusplus
//	}
//#endif



#endif

