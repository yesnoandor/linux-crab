#ifndef	_PMEM_H_
#define	_PMEM_H_

//#ifdef __cplusplus
//	extern "C" {
//#endif

int read_pmem(size_t address,unsigned int &value);
int write_pmem(size_t address,unsigned int value);
int read_pmem(size_t address,unsigned short &value);
int write_pmem(size_t address,unsigned short value);
int read_pmem(size_t address,unsigned char &value);
int write_pmem(size_t address,unsigned char value);

int read_pmem(size_t address,unsigned int arr[],ssize_t size);
int write_pmem(size_t address,unsigned int arr[],ssize_t size);
int read_pmem(size_t address,unsigned short arr[],ssize_t size);
int write_pmem(size_t address,unsigned short arr[],ssize_t size);
int read_pmem(size_t address,unsigned char arr[],ssize_t size);
int write_pmem(size_t address,unsigned char arr[],ssize_t size);


//#ifdef __cplusplus
//	}
//#endif


#endif

