#ifndef _MISC_H
#define	_MISC_H

#ifdef __cplusplus
extern "C" {
#endif

#define		MIN(A,B)			((A)<(B)?(A):(B))
#define		MAX(A,B)			((A)>(B)?(A):(B))

#define		ARRAY_SIZE(arr)		(sizeof(arr) / sizeof((arr)[0]))

#ifndef 	offsetof		// some platform defined in #include <stddef.h>
#define 	offsetof(STRUCTURE,FIELD)		((long)((char*)&((STRUCTURE*)0)->FIELD))
//#define 	offsetof(type,member)				((long)((char*)&((type*)0)->member))
#endif

/* 
* container_of - cast a member of a structure out to the containing structure 
* @ptr:        the pointer to the member. 
* @type:       the type of the container struct this is embedded in. 
* @member:     the name of the member within the struct. 
*/ 
#define container_of(ptr, type, member)\
	({const typeof( ((type *)0)->member ) *__mptr = (ptr);\
	 (type *)((char *)__mptr - offsetof(type,member));})

#ifdef __cplusplus
	 }
#endif
	 
#endif
