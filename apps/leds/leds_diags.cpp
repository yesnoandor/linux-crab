#include		"common.h"
#include		"leds_parse.h"

#define		LEDS_XML		"./leds_diags.xml"

int sysfs_leds_test()
{
	int i;
	int ret = ERROR_NO;
	
	return ret;
}

int main(int argc,char* argv[])
{
	int ret;

	// parse xml
	leds_parse_xml(LEDS_XML);
	leds_info_printf();

	ret = sysfs_leds_test();
	DbgPrintf("sysfs leds test result = %d\r\n",ret);

	return ret;
}

