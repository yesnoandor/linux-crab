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

static const char * node_list[][3] =
{
	{	// channel 1
		"/sys/class/hwmon/hwmon20/in1_input",		// bus voltage
		"/sys/class/hwmon/hwmon20/in4_input",		// shut voltage
		"/sys/class/hwmon/hwmon20/curr1_input",		// current
	},
	{
		"/sys/class/hwmon/hwmon20/in2_input",		// bus voltage
		"/sys/class/hwmon/hwmon20/in5_input",		// shut voltage
		"/sys/class/hwmon/hwmon20/curr2_input",		// current	
	},
	{
		"/sys/class/hwmon/hwmon20/in3_input",		// bus voltage
		"/sys/class/hwmon/hwmon20/in6_input",		// shut voltage
		"/sys/class/hwmon/hwmon20/curr3_input",		// current	
	}
};

static int node_max[3][3] = 
{
	{
		100,
		100,
		200,
	},
	{
		100,
		100,
		200,
	},
	{
		100,
		100,
		200
	}
};

static int node_min[3][3] =
{
	{
		10,
		10,
		20,
	},
	{
		10,
		10,
		20,
	},
	{
		10,
		10,
		20
	}
};

static int node_value[3][3] =
{
	{
		0,
		0,
		0
	},
	{
		0,
		0,
		0
	},
	{
		0,
		0,
		0
	}
};

static int node_err[] =
{
	ERROR_BUS_VOLTAGE,
	ERROR_SHUNT_VOLTAGE,
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
        DbgError("can't open device!(%s)\r\n",node);
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

int sysfs_ina3221_test()
{
	int i,channel;
	
	int ret = ERROR_NO;

	for(channel = 0;channel < 3;channel++)
	{
		for(i=0;i<ARRAY_SIZE(node_list[channel]);i++)
		{
			ret = get_node_item(node_list[channel][i],node_value[channel][i]);
			if(ret != ERROR_NO)
				return ret;

			DbgPrintf("max = 0x%x,min = 0x%x, val = 0x%x\r\n",node_max[channel][i],node_min[channel][i],node_value[channel][i]);
		
			if((node_value[channel][i] > node_max[channel][i]) || (node_value[channel][i] <  node_min[channel][i]))
			{
				ret = node_err[i];
				DbgError("read error!(value = %d,max = %d,min = %d)\r\n",node_value[channel][i],node_max[channel][i],node_min[channel][i]);
			}

			DbgGood("read ok!\r\n");
		}

	}
	
	return ret;
}

int main(int argc,char* argv[])
{
	int ret;

	// parse xml

	ret = sysfs_ina3221_test();
	DbgPrintf("sysfs ina3221 test result = %d\r\n",ret);

	return ret;
}

