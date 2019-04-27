#include	"common.h"

#define		INA2XX_SHUNT_VOLTAGE_NODE		"/sys/class/hwmon/hwmon1/in0_input"
#define		INA2XX_BUS_VOLTAGE_NODE			"/sys/class/hwmon/hwmon1/in1_input"
#define		INA2XX_POWER_VOLTAGE_NODE		"/sys/class/hwmon/hwmon1/power1_input"
#define		INA2XX_CURRENT_NODE				"/sys/class/hwmon/hwmon1/curr1_input"
#define		INA2XX_SHUNT_RESISTANCE_NODE	"/sys/class/hwmon/hwmon1/shunt_resistor"

#define		INA2XX_SHUNT_VOLTAGE_MAX		2000	// mV	
#define		INA2XX_BUS_VOLTAGE_MAX			2000
#define		INA2XX_POWER_VOLTAGE_MAX		2000
#define		INA2XX_CURRENT_MAX				500		// mA

#define		INA2XX_SHUNT_VOLTAGE_MIN		1000
#define		INA2XX_BUS_VOLTAGE_MIN			1000
#define		INA2XX_POWER_VOLTAGE_MIN		1000
#define		INA2XX_CURRENT_MIN				100



enum{
	ERROR_NO = 0,
	ERROR_OPEN = -1,
	ERROR_WRITE = -2,
	ERROR_READ = -3,
	ERROR_COMPARE = -4,
	ERROR_SHUNT_VOLTAGE = -10,
	ERROR_BUS_VOLTAGE = -11,
	ERROR_POWER_VOLTAGE = -12,
	ERROR_CURRENT = -13,
	ERROR_OTHERS = -100
};

static const char * node_list[] =
{
	"/sys/class/leds/led-ds23",		
	"/sys/class/leds/led-ds24",
	"/sys/class/leds/led-ds25",
	"/sys/class/leds/led-ds26"
};

static int node_low[] = 
{
	0,
	0,
	0,
	0
};

static int node_high[] =
{
	255,
	255,
	255,
	255
};




int set_node_item(const char * node,int value)
{
	int fd, size;
	char buf[64]= {0};
	
	DbgFuncEntry();

	fd= open(node,O_RDWR);
    if(fd < 0)
    {
        DbgError("can't open device!(%s)\r\n",node);
        return ERROR_OPEN;
    }

	snprintf(buf, sizeof(buf), "%d",value);

    size = write(fd,buf,strlen(buf));
    if(size < 0)
    {
        DbgError("write error!(size = %d)\r\n",size);
        return ERROR_WRITE;
    }
    
	close(fd);

	DbgFuncExit();
	
    return ERROR_NO;
}

int sysfs_leds_test()
{
	int i;
	int ret = ERROR_NO;
	
	for(i=0;i<ARRAY_SIZE(node_list);i++)
	{
		ret = set_node_item(node_list[i],node_low[i]);
		if(ret != ERROR_NO)
			return ret;
	}

	for(i=0;i<ARRAY_SIZE(node_list);i++)
	{
		ret = set_node_item(node_list[i],node_high[i]);
		if(ret != ERROR_NO)
			return ret;
	}

	return ret;
}

int main(int argc,char* argv[])
{
	int ret;

	// parse xml

	ret = sysfs_leds_test();
	DbgPrintf("sysfs leds test result = %d\r\n",ret);

	return ret;
}

