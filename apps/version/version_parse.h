#ifndef _VERSION_PARSE_H_
#define _VERSION_PARSE_H_



//#ifdef __cplusplus
//	extern "C" {
//#endif

typedef struct{
	int hw_enable;
	string hw_major_node;
	string hw_minor_node;
	int sw_enable;
	string sw_version;
}version_params_t,*p_version_params_t;


void version_parse_xml(const char *file);
void version_info_printf(void);
version_params_t * get_version_params(void);

//#ifdef __cplusplus
//	}
//#endif



#endif

