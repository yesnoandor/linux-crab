#include		"common.h"
#include		"pmem.h"

using namespace std;

template <typename T>
static int write(size_t address,T value)
{
	const char memDevice[] = "/dev/mem";

	unsigned char * map_base;
	int fd;
	ssize_t size;

	fd = open(memDevice, O_RDWR|O_SYNC);
	if (fd < 0)
	{
		DbgError("can't open device!(%s)(errno = %s)\r\n",memDevice,strerror(errno));
		return errno;
	}

	size = sizeof(value);
	map_base = (unsigned char *)mmap((void*) nullptr, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, address);
	if (map_base == MAP_FAILED)
	{
		DbgError("can't mmap device!(errno = %s)\r\n",strerror(errno));
		return errno;
	}
	
	DbgGood("Successful memory map address: 0x%zx size: 0x%zx.\r\n",address,size);
	
	*((T *)map_base) = value;

	close(fd);
	munmap(map_base, size);
}

template <typename T>
static int read(size_t address,T &value)
{
	const char memDevice[] = "/dev/mem";

	unsigned char * map_base;
	int fd;
	ssize_t size;

	fd = open(memDevice, O_RDWR|O_SYNC);
	if (fd < 0)
	{
		DbgError("can't open device!(%s)(errno = %s)\r\n",memDevice,strerror(errno));
		return errno;
	}

	size = sizeof(value);
	map_base =  (unsigned char *)mmap((void*) nullptr, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, address);
	if (map_base == MAP_FAILED)
	{
		DbgError("can't mmap device!(errno = %s)\r\n",strerror(errno));
		return errno;
	}
	
	DbgGood("Successful memory map address: 0x%zx size: 0x%zx.\r\n",address,size);
	
	value = *((T *)map_base);

	close(fd);
	munmap(map_base, size);

	return ERROR_NO;
}

template <typename T>
static int write(size_t address,T arr[],ssize_t size)
{
	const char memDevice[] = "/dev/mem";

	unsigned char * map_base;
	int fd;
	ssize_t byte_size;

	byte_size = size*sizeof(T);
	DbgPrintf("byte size = 0x%lx\r\n",byte_size);

	fd = open(memDevice, O_RDWR|O_SYNC);
	if (fd < 0)
	{
		DbgError("can't open device!(%s)(errno = %s)\r\n",memDevice,strerror(errno));
		return errno;
	}

	map_base =  (unsigned char *)mmap((void*) nullptr, byte_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, address);
	if (map_base == MAP_FAILED)
	{
		DbgError("can't mmap device!(errno = %s)\r\n",strerror(errno));
		return errno;
	}
	
	DbgGood("Successful memory map address: 0x%zx size: 0x%zx.\r\n",address,byte_size);
	
	memcpy(map_base,arr,byte_size);

	close(fd);
	munmap(map_base, byte_size);

	return ERROR_NO;
}

template <typename T>
static int read(size_t addr,T arr[],ssize_t size)
{
	const char memDevice[] = "/dev/mem";

	unsigned char * map_base;
	int fd;
	ssize_t byte_size;

	byte_size = size*sizeof(T);
	DbgPrintf("byte size = 0x%lx\r\n",byte_size);
	
	fd = open(memDevice, O_RDWR|O_SYNC);
	if (fd < 0)
	{
		DbgError("can't open device!(%s)(errno = %s)\r\n",memDevice,strerror(errno));
		return errno;
	}

	map_base =  (unsigned char *)mmap((void*) nullptr, byte_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, addr);
	if (map_base == MAP_FAILED)
	{
		DbgError("can't mmap device!(errno = %s)\r\n",strerror(errno));
		return errno;
	}
	
	DbgGood("Successful memory map address: 0x%zx size: 0x%zx.\r\n",addr,byte_size);
	
	memcpy(arr,map_base,byte_size);

	close(fd);
	munmap(map_base, byte_size);

	return ERROR_NO;
}

//---------------------------------
int read_pmem(size_t address,unsigned int &value)
{
	return read(address,value);
}

int write_pmem(size_t address,unsigned int value)
{
	return write(address,value);
}

int read_pmem(size_t address,unsigned short &value)
{
	return read(address,value);
}

int write_pmem(size_t address,unsigned short value)
{
	return write(address,value);
}

int read_pmem(size_t address,unsigned char &value)
{
	return read(address,value);
}

int write_pmem(size_t address,unsigned char value)
{
	return write(address,value);
}

int read_pmem(size_t address,unsigned int arr[],ssize_t size)
{
	return read<unsigned int>(address,arr,size);
}

int write_pmem(size_t address,unsigned int arr[],ssize_t size)
{
	return write<unsigned int>(address,arr,size);
}

int read_pmem(size_t address,unsigned short arr[],ssize_t size)
{
	return read<unsigned short>(address,arr,size);
}

int write_pmem(size_t address,unsigned short arr[],ssize_t size)
{
	return write<unsigned short>(address,arr,size);
}

int read_pmem(size_t address,unsigned char arr[],ssize_t size)
{
	return read<unsigned char>(address,arr,size);
}

int write_pmem(size_t address,unsigned char arr[],ssize_t size)
{
	return write<unsigned char>(address,arr,size);
}