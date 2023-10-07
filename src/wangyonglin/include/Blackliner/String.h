#if !defined(INCLUDE_BLACKLINER_STRING_H)
#define INCLUDE_BLACKLINER_STRING_H
#include <Blackliner/Skeletons.h>
#include <Blackliner/Unix_timestamp_converter.h>

String *pStringInit(String **deststring, char *valuestring, size_t valuelength);
void pStringFree(String *dest);

uint8_t *StringInit(String *deststring, char *valuestring, size_t valuelength);
uint8_t * StringGetting(String deststring);
void StringDeallocate(String deststring);

void *SkeletonAllocate(void **dest, size_t destsize);
// void *SkeletonInit(void **dest, size_t destsize);
void SkeletonDeallocate(void *dest);



uint8_t *BufferAllocate(uint8_t **dest, size_t destsize);
void BufferDeallocate(uint8_t *dest);
char *BufferInit(char **dest, char *datastring, size_t datalength);
void BufferFree(char *dest);
char *BufferFormat(char **__dest, size_t fmtsize, char *format, ...);
char *BufferCatenate(char *__restrict__ __dest, size_t fmtsize, char *format, ...);

uint8_t *IntegerInit(uint8_t **skeleton, long value);
uint8_t *BooleanInit(uint8_t **skeleton, Boolean value);

#endif