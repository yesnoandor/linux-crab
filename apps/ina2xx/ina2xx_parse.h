#ifndef	INA2XX_PARSE_H_
#define	INA2XX_PARSE_H_


//#ifdef __cplusplus
//	extern "C" {
//#endif

#define		INA2XX_MAX_NODE	3


typedef struct{
	string node_name;
	int min_value;
	int max_value;
}ina2xx_node_params_t,*p_ina2xx_node_params_t;

typedef struct{
	int enable;
	string node_base;
	ina2xx_node_params_t node[INA2XX_MAX_NODE];
}ina2xx_params_t,*p_ina2xx_params_t;


ina2xx_params_t * get_ina2xx_params(void);
void ina2xx_parse_xml(const char *file);
void ina2xx_info_printf(void);


//#ifdef __cplusplus
//	}
//#endif



#endif

