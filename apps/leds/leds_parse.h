#ifndef	_LEDS_PARSE_H_
#define	_LEDS_PARSE_H_

//#ifdef __cplusplus
//	extern "C" {
//#endif





typedef struct{

}leds_params_t,*p_leds_params_t;

void leds_parse_xml(const char *file);
leds_params_t * get_leds_params(void);
void leds_info_printf(void);

//#ifdef __cplusplus
//	}
//#endif



#endif

