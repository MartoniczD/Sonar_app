#ifndef _OS_TYPES
#define _OS_TYPES

#ifndef NULL
 #ifdef __cplusplus
  #define NULL          0
 #else
  #define NULL          ((void *) 0)
 #endif
#endif

#ifndef ABS
	#define ABS(x)         ((x) < 0) ? (-(x)) : (x)
#endif

#ifndef MAX
	#define MAX(a,b)       ((a) < (b)) ? (b) : (a)
#endif

#ifndef MIN
	#define MIN(a,b)       ((a) > (b)) ? (b) : (a)
#endif

/* Variable definitions */
typedef unsigned char 	uint8;
typedef signed   char 	sint8;

typedef unsigned short	uint16;
typedef   signed short	sint16;

typedef unsigned int		uint32;
typedef   signed int		sint32;

typedef unsigned long   uint64;
typedef   signed long   sint64;

typedef enum{
	true 	= 1u,
	false = 0u
}bool;


#endif /* _OS_TYPES */

