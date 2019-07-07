#ifndef _AT24CXX_PARSE_H_
#define _AT24CXX_PARSE_H_


//#ifdef __cplusplus
//	extern "C" {
//#endif

typedef struct{
	int enable;
	string node;
	int offset;
	string str;
}sysfs_params_t,*p_sysfs_params_t;

typedef struct{
	int enable;
	string node;
	int slaveaddr;
	int offset;
	int value;
}devfs_params_t,*p_devfs_params_t;

void at24cxx_parse_xml(const char *file);
void at24cxx_info_printf(void);

sysfs_params_t * get_at24cxx_sysfs_params(void);
devfs_params_t * get_at24cxx_devfs_params(void);

//#ifdef __cplusplus
//	}
//#endif



#endif

