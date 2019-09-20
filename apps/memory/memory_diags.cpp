#include		"common.h"
#include		"memory_parse.h"

using namespace std;

#define		MEMORY_XML		"./memory_diags.xml"

int pl_memory_test()
{
	int i;
	int ret = ERROR_NO;
	unsigned char * pbuf = NULL;
	
	//string node;
	memory_params_t *params;
	unsigned int val1;
	unsigned short val2;
	unsigned char val3;
	
	unsigned int arr1[ ] = {0x11111111,0x22222222,0x3333333,0x44444444};
	unsigned short arr2[ ] = {0x5555,0x6666,0x7777,0x1888};
	unsigned char arr3[ ] = {0x11,0x22,0x33,0x44};
	char temp_buf[256];

	DbgFuncEntry();

	params = get_memory_params();

	#if 0
	write_pmem(params->address,(unsigned int)0x55aa1188);
	read_pmem(params->address,val1);
	DbgPrintf("value1 = 0x%lx\r\n",val1);

	write_pmem(params->address,(unsigned short)0x5678);
	read_pmem(params->address,val2);
	DbgPrintf("value2 = 0x%lx\r\n",val2);

	write_pmem(params->address,(unsigned char)0x56);
	read_pmem(params->address,val3);
	DbgPrintf("value3 = 0x%lx\r\n",val3);


	write_pmem(params->address,arr1,4);
	memset(temp_buf,0x00,sizeof(temp_buf));
	read_pmem(params->address,(unsigned int *)temp_buf,4);
	for(i=0;i<4;i++)
	{
		DbgPrintf("temp_buf[%d] = 0x%x\r\n",i,*(int *)(temp_buf + sizeof(int) * i));
	}
	
	write_pmem(params->address,arr2,4);
	memset(temp_buf,0x00,sizeof(temp_buf));
	read_pmem(params->address,(unsigned short *)temp_buf,4);
	for(i=0;i<4;i++)
	{
		DbgPrintf("temp_buf[%d] = 0x%x\r\n",i,*(short *)(temp_buf + sizeof(short)*i));
	}
	
	write_pmem(params->address,arr3,4);
	memset(temp_buf,0x00,sizeof(temp_buf));
	read_pmem(params->address,(unsigned char *)temp_buf,4);
	for(i=0;i<4;i++)
	{
		DbgPrintf("temp_buf[%d] = 0x%x\r\n",i,*(char *)(temp_buf + sizeof(char) * i));
	}
	#endif

	pbuf = (unsigned char *)malloc(params->size);
	for(i=0;i<params->size;i++)
	{
		pbuf[i] = i;
	}
	write_pmem(params->address,pbuf,params->size);
	memset(pbuf,0x00,params->size);
	read_pmem(params->address,pbuf,params->size);
	for(i=0;i<params->size;i++)
	{
		if(pbuf[i] != (i % 256))
		{
			DbgError("mismatch read and write pl memory (index=%d,value=0x%x)\r\n",i,pbuf[i]);
			free(pbuf);
			return -1;
		}
	}

	DbgGood("PL MEMORY TEST OK!\r\n");
	free(pbuf);
	
	DbgFuncExit();
	
	return ret;
}



int main(int argc,char* argv[])
{
	int ret;

	// parse xml
	memory_parse_xml(MEMORY_XML);
	memory_info_printf();

	ret = pl_memory_test();
	DbgPrintf("sysfs memory test result = %d\r\n",ret);

	return ret;
}
