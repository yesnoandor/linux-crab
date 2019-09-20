#include		"common.h"
#include		"at24cxx_parse.h"

#define		AT24CXX_XML		"./at24cxx_diags.xml"

int sysfs_eeprom_test(p_sysfs_params_t params)
{
	int fd, size, len;
	char buf[50]= {0};

	//DbgFuncEntry();

	len = strlen(params->str.c_str());
	//DbgPrintf("len = %d\r\n",len);

	//
	fd = open(params->node.c_str(),O_RDWR);
	if(fd < 0)
	{
		DbgError("can't open device!(%s)(errno = %s)\r\n",params->node.c_str(),strerror(errno));
		return errno;
	}

	// д����
	lseek(fd,params->offset,SEEK_SET);
	size = write(fd,params->str.c_str(),len);
	if(size != len)
	{
		DbgError("write error!(size = %d,len = %d)(errno = %s)\r\n",size,len,strerror(errno));
		return errno;
	}
	//DbgGood("write ok!\r\n");

	// �建��
	memset(buf,0x00,sizeof(buf));

	// ������
	lseek(fd,params->offset,SEEK_SET);
	size=read(fd,buf,len);
	if(size != len)
	{
		DbgError("read error!(size = %d,len = %d)(errno = %s)\r\n",size,len,strerror(errno));
		return errno;
	}
	//DbgGood("read ok!\r\n");

	// �Ƚ���֤
	//DbgGood("%s\r\n",params->str.c_str());
	//DbgGood("%s\r\n",buf);
	
	if(strcmp(buf,params->str.c_str()) != 0)
	{
		DbgError("compare error!(%s)\r\n",buf);
		return -ERROR_MODULE_EERPOM;
	}
	//DbgGood("compare ok!\r\n");

	// 
	close(fd);

	//DbgFuncExit();

	return ERROR_NO;
}


int devfs_eeprom_test(p_devfs_params_t params)
{
	int fd,ret;
	struct i2c_rdwr_ioctl_data e2prom_data;
	unsigned char temp;
	//unsigned char reg_addr,reg_data;

	//DbgFuncEntry();

	// ��eeprom  �豸�ļ����
	fd = open(params->node.c_str(),O_RDWR);
	if(fd < 0)
	{
		DbgError("can't open device!(%s)strerror(errno)\r\n",params->node.c_str(),strerror(errno));
		return errno;
	}	

	//slave_addr = 0x54;
	//reg_addr = TEST_OFFSET_ADDR;
	//reg_data = 0x5a;
	
	// nmsgs�������ж���start�ź�
	// һ��msgs��Ӧһ��start�ź�
	// ��nmsg���źŽ��������߻����һ��stop
	// ������Ϊ�ڲ���ʱ��������õ�2��start�ź�(�ֽڶ�������)
	e2prom_data.nmsgs=2;
	e2prom_data.msgs=(struct i2c_msg*)malloc(e2prom_data.nmsgs*sizeof(struct i2c_msg));//����ռ�
	if(!e2prom_data.msgs)
	{
		DbgError("malloc memory not enough!strerror(errno)\r\n",strerror(errno));
		return errno;
	}

	ioctl(fd,I2C_TIMEOUT,1);	//��ʱʱ��
	ioctl(fd,I2C_RETRIES,2);		//�ظ�����

	/*
	* write eeprom
	* 	1. дeeprom��Ҫһ����Ϣ
	*       2 .����Ϊ2���ֽ�
	*       	��һ���ֽ���Ҫд�����ݵĵ�ַ
	*       	�ڶ����ֽ���Ҫд�������
	*/
	e2prom_data.nmsgs = 1;
	(e2prom_data.msgs[0]).len = 2;
	(e2prom_data.msgs[0]).addr = params->slaveaddr;
	(e2prom_data.msgs[0]).flags = 0; // write flag
	(e2prom_data.msgs[0]).buf = (unsigned char*)malloc(2);
	(e2prom_data.msgs[0]).buf[0] = params->offset;
	(e2prom_data.msgs[0]).buf[1] = params->value;
	ret = ioctl(fd,I2C_RDWR,(unsigned long)&e2prom_data);
	if(ret<0)
	{
		DbgError("write ioctl error!(errno = %s)\r\n",strerror(errno));
		return errno;
	}

	sleep(1);

	/*
	* read eeprom
	* 	1. ��eeprom ��Ҫ������Ϣ
	*       2. ��һ����Ϣʵ����дeeprom �ĵ�ַ,     ����Ϊ1���ֽ�
	*       3. �ڶ�����Ϣ���Ƕ�eeprom
	*/
	e2prom_data.nmsgs=2;
	(e2prom_data.msgs[0]).len = 1;
	(e2prom_data.msgs[0]).addr = params->slaveaddr;
	(e2prom_data.msgs[0]).flags = 0; // write flag
	(e2prom_data.msgs[0]).buf[0] = params->offset;

	(e2prom_data.msgs[1]).len = 1;
	(e2prom_data.msgs[1]).addr = params->slaveaddr;
	(e2prom_data.msgs[1]).flags = I2C_M_RD;	// read flag
	(e2prom_data.msgs[1]).buf = &temp;	//��ŷ���ֵ�ĵ�ַ
	(e2prom_data.msgs[1]).buf[0] = 0;
	ret = ioctl(fd,I2C_RDWR,(unsigned long)&e2prom_data);
	if(ret < 0)
	{
		DbgError("read ioctl error!(errno = %s)\r\n",strerror(errno));
		return errno;
	}

	// �Ƚ���֤
	if((e2prom_data.msgs[1]).buf[0] != params->value)
	{
		DbgError("compare error!(0x%x : 0x%x)\r\n",(e2prom_data.msgs[1]).buf[0],params->value);
		return -ERROR_MODULE_EERPOM;
	}
	//DbgGood("compare ok!\r\n");

	close(fd);

	free((e2prom_data.msgs[0]).buf);
	free(e2prom_data.msgs);

	DbgFuncExit();
	
	return ERROR_NO;
}

int main(int argc,char* argv[])
{
	int ret;
	p_sysfs_params_t sysfs_params;
	p_devfs_params_t devfs_params;

	// at24xcc parse xml
	at24cxx_parse_xml(AT24CXX_XML);
	//at24cxx_info_printf();

	sysfs_params = get_at24cxx_sysfs_params();
	devfs_params = get_at24cxx_devfs_params();

	if(sysfs_params->enable)
	{
		ret = sysfs_eeprom_test(sysfs_params);
		//DbgPrintf("sysfs eeprom test result = %d\r\n",ret);
		if(!ret)
		{
			DbgGood("\r\n\r\n");
			DbgGood("=========================\r\n");
			DbgGood("=========================\r\n");
			DbgGood("sysfs eeprom test ok!!!\r\n");
			DbgGood("=========================\r\n");
			DbgGood("=========================\r\n");
			DbgGood("\r\n\r\n");
		}
		else
		{
			DbgError("\r\n\r\n");
			DbgError("=========================\r\n");
			DbgError("=========================\r\n");
			DbgError("sysfs eeprom test fail!!!\r\n");
			DbgError("=========================\r\n");
			DbgError("=========================\r\n");
			DbgError("\r\n\r\n");
		}
	}
	
	sleep(1);

	if(devfs_params->enable)
	{
		ret = devfs_eeprom_test(devfs_params);
		DbgPrintf("devfs i2c test result = %d\r\n",ret);
	}
	
	return ret;
}



