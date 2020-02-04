#ifndef _ASSERT_

#define _ASSERT_


#ifndef  _DEBUG

#define  ASSERT(x)  

#else
#include <stdint.h>
extern void DbgPrintf(uint8_t * strFile,uint32_t dwNo);
#define  ASSERT(x)    { if(x){\
			DbgPrintf(__FILE__,__LINE__);\
			while(1);}\
		      }

#endif




#endif
