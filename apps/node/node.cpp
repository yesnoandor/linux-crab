#include		"common.h"
#include		"node.h"

int get_node_item(const char * node,int &value)
{
	int fd, size;
	char buf[64]= {0};
	
	DbgFuncEntry();

	fd= open(node,O_RDONLY);
	if(fd < 0)
	{
		DbgError("can't open device!(%s)(errno = %s)\r\n",node,strerror(errno));
		return errno;
	}

	size=read(fd,buf,sizeof(buf));
	if(size < 0)
	{
		DbgError("read error!(size = %d)(errno = %s)\r\n",size,strerror(errno));
		return errno;
	}
	value = strtol(buf,NULL,0);

	close(fd);

	DbgFuncExit();

	return ERROR_NO;
}

int set_node_item(const char * node,int value)
{
	int fd, size;
	char buf[64]= {0};
	
	DbgFuncEntry();

	fd= open(node,O_RDWR);
	if(fd < 0)
	{
		DbgError("can't open device!(%s)(errno = %s)\r\n",node,strerror(errno));
		return errno;
	}
	
	snprintf(buf, sizeof(buf), "%d",value);
	
	size = write(fd,buf,strlen(buf));
	if(size < 0)
	{
		DbgError("write error!(size = %d)(errno = %s)\r\n",size,strerror(errno));
		return errno;
	}
	
	close(fd);
	
	DbgFuncExit();
	
	return ERROR_NO;
}


int get_node_item(const char * node,std::string &str)
{
	int fd, size;
	char buf[64]= {0};
	
	DbgFuncEntry();

	fd= open(node,O_RDONLY);
	if(fd < 0)
	{
		DbgError("can't open device!(%s)(errno = %s)\r\n",node,strerror(errno));
		return errno;
	}

	size=read(fd,buf,sizeof(buf));
	if(size < 0)
	{
		DbgError("read error!(size = %d)(errno = %s)\r\n",size,strerror(errno));
		return errno;
	}
	str = buf;
	
	close(fd);

	DbgFuncExit();

	return ERROR_NO;
}

int set_node_item(const char * node,std::string str)
{
	int fd, size;
	char buf[64]= {0};
	
	DbgFuncEntry();

	fd= open(node,O_RDWR);
	if(fd < 0)
	{
		DbgError("can't open device!(%s)(errno = %s)\r\n",node,strerror(errno));
		return errno;
	}
	
	snprintf(buf, sizeof(buf), "%s",str.c_str());
	
	size = write(fd,buf,strlen(buf));
	if(size < 0)
	{
		DbgError("write error!(size = %d)(errno = %s)\r\n",size,strerror(errno));
		return errno;
	}
	
	close(fd);
	
	DbgFuncExit();
	
	return ERROR_NO;
}
