#ifndef _DEBUG_H_
#define _DEBUG_H_

#include	<string>
//#include	<vector>

//#include	"stdio.h"
//#include	"string.h"
#include	<stdio.h>
#include 	<stdarg.h>



using namespace std;

inline string className(const std::string& prettyFunction)
{
	size_t colons = prettyFunction.find("::");

	if(colons == std::string::npos)
		return "";
		
	size_t begin = prettyFunction.substr(0,colons).rfind(" ") + 1;
	size_t end = colons - begin;

	return prettyFunction.substr(begin,end);
}

#define	__CLASS__ 	className(__PRETTY_FUNCTION__).c_str()


#define DBG_C_NONE         "\033[m"
#define DBG_C_RED          "\033[0;32;31m"
#define DBG_C_LIGHT_RED    "\033[1;31m"
#define DBG_C_GREEN        "\033[0;32;32m"
#define DBG_C_LIGHT_GREEN  "\033[1;32m"
#define DBG_C_BLUE         "\033[0;32;34m"
#define DBG_C_LIGHT_BLUE   "\033[1;34m"
#define DBG_C_DARY_GRAY    "\033[1;30m"
#define DBG_C_CYAN         "\033[0;36m"
#define DBG_C_LIGHT_CYAN   "\033[1;36m"
#define DBG_C_PURPLE       "\033[0;35m"
#define DBG_C_LIGHT_PURPLE "\033[1;35m"
#define DBG_C_BROWN        "\033[0;33m"
#define DBG_C_YELLOW       "\033[1;33m"
#define DBG_C_LIGHT_GRAY   "\033[0;37m"
#define DBG_C_WHITE        "\033[1;37m"

#define MAX_DBG_BUFFER_SIZE		256

#ifdef	DEBUG_Y

#define	DbgFuncEntry()								\
	printf(DBG_C_DARY_GRAY);						\
	printf("%s::%s:++++++++++\r\n",__CLASS__,__FUNCTION__);		\
	printf(DBG_C_NONE);								\
	fflush(stdout);


#define	DbgFuncExit()								\
	printf(DBG_C_DARY_GRAY);						\
	printf("%s::%s:----------\r\n",__CLASS__,__FUNCTION__);		\
	printf(DBG_C_NONE);								\
	fflush(stdout);

inline void DbgPrintf(const char *fmt, ...)
{
	char strings[MAX_DBG_BUFFER_SIZE];

	va_list args;
	va_start(args, fmt);
	vsprintf(strings, fmt, args);
	va_end(args);

	printf("%s", strings);
	printf(DBG_C_NONE);
	fflush(stdout);
}

inline void DbgGood(const char *fmt, ...)
{
	char strings[MAX_DBG_BUFFER_SIZE];

	va_list args;
	va_start(args, fmt);
	vsprintf(strings, fmt, args);
	va_end(args);

	printf(DBG_C_LIGHT_BLUE);
	printf("%s",strings);
	printf(DBG_C_NONE);

	fflush(stdout);
}
#else

#define	DbgFuncEntry()
#define	DbgFuncExit()

#define DbgPrintf(format, args...)					({}) 
#define DbgGood(format, args...)					({}) 
#endif

inline void DbgError(const char *fmt, ...)
{
	char strings[MAX_DBG_BUFFER_SIZE];

	va_list args;
	va_start(args, fmt);
	vsprintf(strings, fmt, args);
	va_end(args);

	printf(DBG_C_LIGHT_RED);
	printf("%s", strings);
	printf(DBG_C_NONE);

	fflush(stdout);
}

inline void DbgWarn(const char *fmt, ...)
{
	char strings[MAX_DBG_BUFFER_SIZE];
	
	va_list args;
	va_start(args, fmt);
	vsprintf(strings, fmt, args);
	va_end(args);

	printf(DBG_C_YELLOW);
	printf("%s",strings);
	printf(DBG_C_NONE);

	fflush(stdout);
}

#endif
