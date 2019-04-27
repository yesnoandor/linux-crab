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
	"/sys/class/hwmon/hwmon1/in0_input",		// in0_input
	"/sys/class/hwmon/hwmon1/in1_input",		// in1_input
	"/sys/class/hwmon/hwmon1/power1_input",		// power1_input
	"/sys/class/hwmon/hwmon1/curr1_input"		// curr1_input
};

static int node_max[] = 
{
	600,
	700,
	50,
	350
};

static int node_min[] =
{
	500,
	600,
	0,
	250
};

static int node_value[] =
{
	0,
	0,
	0,
	0
};

static int node_err[] =
{
	ERROR_SHUNT_VOLTAGE,
	ERROR_BUS_VOLTAGE,
	ERROR_POWER_VOLTAGE,
	ERROR_CURRENT
};




int get_node_item(const char * node,int &value)
{
	int fd, size;
	char buf[64]= {0};
	
	DbgFuncEntry();

	// 
    fd= open(node,O_RDONLY);
    if(fd< 0)
    {
        DbgError("can't open device!(%s)\r\n",INA2XX_SHUNT_VOLTAGE_NODE);
        return ERROR_OPEN;
    }

	
	// ¶Á²Ù×÷
    size=read(fd,buf,sizeof(buf));
    if(size < 0)
    {
        DbgError("read error!(size = %d)\r\n",size);
        return ERROR_READ;
    }
    
    value = strtol(buf,NULL,0);

	close(fd);

	DbgFuncExit();
	
    return ERROR_NO;
}

int sysfs_ina2xx_test()
{
	int i;
	int ret = ERROR_NO;
	
	for(i=0;i<ARRAY_SIZE(node_list);i++)
	{
		ret = get_node_item(node_list[i],node_value[i]);
		if(ret != ERROR_NO)
			return ret;

		DbgPrintf("max = 0x%x,min = 0x%x, val = 0x%x\r\n",node_max[i],node_min[i],node_value[i]);
		
		if((node_value[i] > node_max[i]) || (node_value[i] <  node_min[i]))
		{
			ret = node_err[i];
			DbgError("read error!(value = %d,max = %d,min = %d)\r\n",node_value[i],node_max[i],node_min[i]);
		}

		DbgGood("read ok!\r\n");
	}

	return ret;
}

int main(int argc,char* argv[])
{
	int ret;

	// parse xml

	ret = sysfs_ina2xx_test();
	DbgPrintf("sysfs ina2xx test result = %d\r\n",ret);

	return ret;
}
