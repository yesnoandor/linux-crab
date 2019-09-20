#ifndef	MEMORY_PARSE_H_
#define	MEMORY_PARSE_H_



//#ifdef __cplusplus
//	extern "C" {
//#endif

typedef struct{
	size_t address;
	ssize_t size;
}memory_params_t,*p_memory_params_t;


memory_params_t * get_memory_params(void);
void memory_parse_xml(const char *file);
void memory_info_printf(void);

//#ifdef __cplusplus
//	}
//#endif



#endif

