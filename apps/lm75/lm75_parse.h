#ifndef	_LM75_PARSE_H_
#define	_LM75_PARSE_H_

//#ifdef __cplusplus
//	extern "C" {
//#endif

typedef struct{
	string name;
	int min_value;
	int max_value;
}lm75_node_params_t,*p_lm75_node_params_t;

typedef struct{
	string node_base;
	lm75_node_params_t node;
}lm75_params_t,*p_lm75_params_t;

lm75_params_t * get_lm75_params(void);
void lm75_parse_xml(const char *file);
void lm75_info_printf(void);


//#ifdef __cplusplus
//	}
//#endif



#endif

