#include <stdint.h>
#include <stdio.h>

#ifdef _DEBUG
void DbgPrintf(uint8_t *strFile, uint32_t wNo)
{
	printf("Wrong parameters value: file %s on line %d\r\n", strFile, wNo);
	while(1);
}
#endif
