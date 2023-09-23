#if !defined(INCLUDE_BLACKLINER_STRING_H)
#define INCLUDE_BLACKLINER_STRING_H
#include <Blackliner/Skeletons.h>

/**
 * initialCapacity
 */

#include <Blackliner/Unix_timestamp_converter.h>


#define TrueBoolean ((Boolean)(true))
#define FalseBoolean ((Boolean)(false))



typedef int Exception;
#define OkException ((Exception)(0))
#define NullPointerException ((Exception)(-1))
#define RuntimeExceion ((Exception)(-2))
#define ArithmeticException ((Exception)(-3))
#define IllegalArgumentException ((Exception)(-4))

typedef enum __Category_t
{
    StringCategory,
    BooleanCategory,
    IntegerCategory
} Category;

typedef struct __DataBuffer
{
    uint8_t *ptr;
    size_t size; ///< size left in the buffer
} DataBuffer;

typedef struct __DataString
{
    size_t datalength;
    char *datastring;
} DataString;

/*
String
*/
char *StringRegister(char **deststring, char *datastring, size_t datalength);
void StringDestory(char *deststring);
DataString DataStringRegister(char *datastring, size_t datalength);
void DataStringDestory(DataString deststring);

/*
Buffer
*/
char *BufferRegister(uint8_t **deststring, uint8_t *datastring, size_t datalength);
void BufferDestory(uint8_t *deststring);
DataBuffer DataBufferRegister(uint8_t *databuffer, size_t datalength);
void DataBufferDestory(DataBuffer databuffer);
#endif