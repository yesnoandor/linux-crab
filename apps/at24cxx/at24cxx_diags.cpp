#include	"common.h"

#define		SYSFS_DEVICE_NAME		"/sys/bus/i2c/devices/7-0054/eeprom"
#define		DEVFS_DEVICE_NAME		"/dev/i2c-7"

#define		TEST_STRING				"EEPROM Test:01234567890!"
#define		TEST_OFFSET_ADDR		0x80

enum{
	ERROR_NO = 0,
	ERROR_OPEN = -1,
	ERROR_WRITE = -2,
	ERROR_READ = -3,
	ERROR_COMPARE = -4,
	ERROR_OTHERS = -100
};

int sysfs_eeprom_test()
{
	int fd, size, len;
	char buf[50]= {0};
	
	DbgFuncEntry();

	len = sizeof(TEST_STRING);

	// 
    fd= open(SYSFS_DEVICE_NAME,O_RDWR);
    if(fd< 0)
    {
        DbgError("can't open device!(%s)\r\n",SYSFS_DEVICE_NAME);
        return ERROR_OPEN;
    }

	// д����
	lseek(fd,TEST_OFFSET_ADDR,SEEK_SET);
	size = write(fd,TEST_STRING,len);
	if(size != len)
	{
        DbgError("write error!(size = %d,len = %d)\r\n",size,len);
        return ERROR_WRITE;
    }
    DbgGood("write ok!\r\n");

	// �建��
    memset(buf,0x00,sizeof(buf));

	// ������
    lseek(fd,TEST_OFFSET_ADDR, SEEK_SET);
    size=read(fd,buf,len);
    if(size != len)
    {
        DbgError("read error!(size = %d,len = %d)\r\n",size,len);
        return ERROR_READ;
    }
    DbgGood("read ok!\r\n");

	// �Ƚ���֤
	if(strcmp(buf,TEST_STRING) != 0)
	{
		DbgError("compare error!(%s)\r\n",buf);
        return ERROR_COMPARE;
	}
	DbgGood("compare ok!\r\n");

	//
    close(fd);

	DbgFuncExit();
	
    return ERROR_NO;
}


int devfs_eeprom_test()
{
	int fd,ret;
	struct i2c_rdwr_ioctl_data e2prom_data;
	unsigned char slave_addr,reg_addr,reg_data;

	DbgFuncEntry();

	// ��eeprom  �豸�ļ����
	fd=open(DEVFS_DEVICE_NAME,O_RDWR);
	if(fd<0)
	{
		DbgError("can't open device!(%s)\r\n",DEVFS_DEVICE_NAME);
		return ERROR_OPEN;
	}	

	slave_addr = 0x54;
	reg_addr = TEST_OFFSET_ADDR;
	reg_data = 0x5a;
	
	// nmsgs�������ж���start�ź�
	// һ��msgs��Ӧһ��start�ź�
	// ��nmsg���źŽ��������߻����һ��stop
	// ������Ϊ�ڲ���ʱ��������õ�2��start�ź�(�ֽڶ�������)
	e2prom_data.nmsgs=2;
	e2prom_data.msgs=(struct i2c_msg*)malloc(e2prom_data.nmsgs*sizeof(struct i2c_msg));//����ռ�
	if(!e2prom_data.msgs)
	{
		DbgError("malloc memory not enough!\r\n");
		return ERROR_OTHERS;
	}

	ioctl(fd,I2C_TIMEOUT,1);	//��ʱʱ��
	ioctl(fd,I2C_RETRIES,2);	//�ظ�����

	/*
	* write eeprom
	* 	1. дeeprom��Ҫһ����Ϣ
	*       2 .����Ϊ2���ֽ�
	*       	��һ���ֽ���Ҫд�����ݵĵ�ַ
	*       	�ڶ����ֽ���Ҫд�������
	*/
	e2prom_data.nmsgs = 1;
	(e2prom_data.msgs[0]).len = 2;
	(e2prom_data.msgs[0]).addr = slave_addr;
	(e2prom_data.msgs[0]).flags = 0; // write flag
	(e2prom_data.msgs[0]).buf = (unsigned char*)malloc(2);
	(e2prom_data.msgs[0]).buf[0] = reg_addr;
	(e2prom_data.msgs[0]).buf[1] = reg_data;
	ret = ioctl(fd,I2C_RDWR,(unsigned long)&e2prom_data);
	if(ret<0)
	{
		DbgError("write ioctl error!(errno = %s)\r\n",strerror(errno));
		return ERROR_WRITE;
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
	(e2prom_data.msgs[0]).addr = slave_addr;
	(e2prom_data.msgs[0]).flags = 0; // write flag
	(e2prom_data.msgs[0]).buf[0] = reg_addr;

	(e2prom_data.msgs[1]).len = 1;
	(e2prom_data.msgs[1]).addr = slave_addr;
	(e2prom_data.msgs[1]).flags = I2C_M_RD;	// read flag
	(e2prom_data.msgs[1]).buf = (unsigned char*)malloc(1);	//��ŷ���ֵ�ĵ�ַ
	(e2prom_data.msgs[1]).buf[0] = 0;
	ret = ioctl(fd,I2C_RDWR,(unsigned long)&e2prom_data);
	if(ret < 0)
	{
		DbgError("read ioctl error!(errno = %s)\r\n",strerror(errno));
		return ERROR_READ;
	}

	// �Ƚ���֤
	if((e2prom_data.msgs[1]).buf[0] != reg_data)
	{
		DbgError("compare error!(%d : %d)\r\n",(e2prom_data.msgs[1]).buf[0],reg_data);
        return ERROR_COMPARE;
	}
	DbgGood("compare ok!\r\n");

	close(fd);

	return ERROR_NO;
}

int main(int argc,char* argv[])
{
	int ret;

	// parse xml

	ret = sysfs_eeprom_test();
	DbgPrintf("sysfs eeprom test result = %d\r\n",ret);

	sleep(1);
	
	ret = devfs_eeprom_test();
	DbgPrintf("devfs i2c test result = %d\r\n",ret);
	return ret;
}



