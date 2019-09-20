#include		"common.h"
#include		"version_parse.h"

#define		DEFAULT_HW_VERSION		"0.0"
#define		DEFAULT_SW_VERSION		"1.0.20190101.08.08.08"
#define		SW_MAJOR_VERSION		1
#define		SW_MINOR_VERSION		1

#define		VERSION_XML		"./version.xml"

void get_hw_version(char * buff)
{
	int ret;
	int major,minor;
	version_params_t * params;
	
	params = get_version_params();

	//DbgPrintf("major node = %s\r\n",params->hw_major_node.c_str());	
	ret = get_node_item(params->hw_major_node.c_str(),major);
	if(ret != ERROR_NO)
	{
		goto get_hw_version_err;
		return;
	}

	//DbgPrintf("minor node = %s\r\n",params->hw_minor_node.c_str());
	ret = get_node_item(params->hw_minor_node.c_str(),minor);
	if(ret != ERROR_NO)
	{
		goto get_hw_version_err;
		return;
	}

	//DbgPrintf("major = %d,minor = %d\r\n",major,minor);
	sprintf(buff,"%d.%d",major+1,minor); 
	return;
	
get_hw_version_err:	
	strcpy(buff,(char *)DEFAULT_HW_VERSION);
	return;
}

/* 
获取程序编译时间, 用于自动生成版本号很好 
 __DATE__  : "Mar 25 2013" 
          	   "Mar  1 2013" 
 
__TIME__  :  "21:42:20" 
*/
void get_sw_version(char * buff)
{
	//memset(buff,0x00,MAX_SW_VERSION_SIZE);
	//memcpy(buff,TEST_SW_VERSION,strlen(TEST_SW_VERSION));char date[MAX_CMD_CHARS_SIZE];
	char date[MAX_TEMP_CHARS_SIZE];
	char time[MAX_TEMP_CHARS_SIZE];
	int year,month,mday;
	int hour,mintue,second;
	int i;
	
	char const * monthes[] =  {
		"Jan",    
		"Feb",
		"Mar",
		"Apr",
		"May",
		"Jun",
		"Jul",
		"Aug",
		"Sep",
		"Oct",
		"Nov", 
		"Dec"
	};

	strcpy(date,__DATE__);
	strcpy(time,__TIME__);

	//DbgPrintf("date = %s\r\n",date);
	//DbgPrintf("time = %s\r\n",time);

	year = strtol(date+strlen(date)-4, 0, 10);  

	month = 0;
	for(i = 0; i < 12; i++)
	{
		if(strstr(date,monthes[i]))
		{
			month = i + 1;
			break;
		}
	}     
	
	date[6] = '\0';
	mday = strtol(date+4, 0, 10); 
	
	//DbgPrintf("year = %d\r\n",year);
	//DbgPrintf("month = %d\r\n",month);
	//DbgPrintf("mday = %d\r\n",mday);

	time[2] = time[5] = '\0';
	hour = strtol(time, 0, 10);
	mintue = strtol(time + 3, 0, 10);
	second = strtol(time + 6, 0, 10);
	
	//DbgPrintf("hour = %d\r\n",hour);
	//DbgPrintf("mintue = %d\r\n",mintue);
	//DbgPrintf("second = %d\r\n",second);
	
	sprintf(buff,"%d.%d.%04d%02d%02d.%02d%02d%02d",SW_MAJOR_VERSION,SW_MINOR_VERSION,year, month, mday, hour,mintue,second); 
}

int main(int argc,char* argv[])
{
	char sw_version[MAX_TEMP_CHARS_SIZE];
	char hw_version[MAX_TEMP_CHARS_SIZE];

	//DbgFuncEntry();
	
	version_parse_xml(VERSION_XML);
	//version_info_printf();

	get_hw_version(hw_version);
	get_sw_version(sw_version);
	
	DbgGood("\r\n\r\n");
	DbgGood("=========================\r\n");
	DbgGood("=========================\r\n");
	DbgGood("NFSI hw version = %s\r\n",hw_version);
	DbgGood("NFSI sw version = %s\r\n",sw_version);	
	DbgGood("=========================\r\n");
	DbgGood("=========================\r\n");
	DbgGood("\r\n\r\n");

	//DbgFuncExit();

	return 0;
}

