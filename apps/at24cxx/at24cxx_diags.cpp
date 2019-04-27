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

	// 写操作
	lseek(fd,TEST_OFFSET_ADDR,SEEK_SET);
	size = write(fd,TEST_STRING,len);
	if(size != len)
	{
        DbgError("write error!(size = %d,len = %d)\r\n",size,len);
        return ERROR_WRITE;
    }
    DbgGood("write ok!\r\n");

	// 清缓冲
    memset(buf,0x00,sizeof(buf));

	// 读操作
    lseek(fd,TEST_OFFSET_ADDR, SEEK_SET);
    size=read(fd,buf,len);
    if(size != len)
    {
        DbgError("read error!(size = %d,len = %d)\r\n",size,len);
        return ERROR_READ;
    }
    DbgGood("read ok!\r\n");

	// 比较验证
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

	// 打开eeprom  设备文件结点
	fd=open(DEVFS_DEVICE_NAME,O_RDWR);
	if(fd<0)
	{
		DbgError("can't open device!(%s)\r\n",DEVFS_DEVICE_NAME);
		return ERROR_OPEN;
	}	

	slave_addr = 0x54;
	reg_addr = TEST_OFFSET_ADDR;
	reg_data = 0x5a;
	
	// nmsgs决定了有多少start信号
	// 一个msgs对应一个start信号
	// 在nmsg个信号结束后总线会产生一个stop
	// 下面因为在操作时序中最多用到2个start信号(字节读操作中)
	e2prom_data.nmsgs=2;
	e2prom_data.msgs=(struct i2c_msg*)malloc(e2prom_data.nmsgs*sizeof(struct i2c_msg));//分配空间
	if(!e2prom_data.msgs)
	{
		DbgError("malloc memory not enough!\r\n");
		return ERROR_OTHERS;
	}

	ioctl(fd,I2C_TIMEOUT,1);	//超时时间
	ioctl(fd,I2C_RETRIES,2);	//重复次数

	/*
	* write eeprom
	* 	1. 写eeprom需要一条消息
	*       2 .长度为2个字节
	*       	第一个字节是要写入数据的地址
	*       	第二个字节是要写入的数据
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
	* 	1. 读eeprom 需要两条消息
	*       2. 第一条消息实际是写eeprom 的地址,     长度为1个字节
	*       3. 第二条消息才是读eeprom
	*/
	e2prom_data.nmsgs=2;
	(e2prom_data.msgs[0]).len = 1;
	(e2prom_data.msgs[0]).addr = slave_addr;
	(e2prom_data.msgs[0]).flags = 0; // write flag
	(e2prom_data.msgs[0]).buf[0] = reg_addr;

	(e2prom_data.msgs[1]).len = 1;
	(e2prom_data.msgs[1]).addr = slave_addr;
	(e2prom_data.msgs[1]).flags = I2C_M_RD;	// read flag
	(e2prom_data.msgs[1]).buf = (unsigned char*)malloc(1);	//存放返回值的地址
	(e2prom_data.msgs[1]).buf[0] = 0;
	ret = ioctl(fd,I2C_RDWR,(unsigned long)&e2prom_data);
	if(ret < 0)
	{
		DbgError("read ioctl error!(errno = %s)\r\n",strerror(errno));
		return ERROR_READ;
	}

	// 比较验证
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



