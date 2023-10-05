#include <Blackliner/Skeletons.h>

void *SkeletonAllocate(void **dest, size_t destsize)
{

    if ((*dest) = global_hooks.allocate(destsize))
    {

        if (memset((*dest), 0x00, destsize))
        {
            return (*dest);
        }
    }
    return (*dest);
}

void SkeletonDeallocate(void *dest)
{
    if (dest)
    {
        free(dest);
    }
}

void *SkeletonInit(void **dest, size_t destsize)
{
    if ((*dest) = global_hooks.allocate(destsize))
    {
        if (memset((*dest), 0x00, destsize))
        {
            return (*dest);
        }
    }
    return (*dest);
}

void SkeletonFree(void *dest)
{
    if (dest)
    {
        free(dest);
    }
}
String *pStringInit(String **deststring, char *valuestring, size_t valuelength)
{
    String *dest = NULL;
    if ((dest) = SkeletonAllocate((void **)deststring, sizeof(String)))
    {
        dest->valuestring = NULL;
        dest->valuelength = 0;
        if (valuelength >= 0)
        {
            if ((dest->valuestring = global_hooks.allocate(valuelength + 1)))
            {
                memset(dest->valuestring, 0x00, valuelength + 1);
                memcpy(dest->valuestring, valuestring, valuelength);
                dest->valuelength = valuelength;
                return dest;
            }
        }
        return dest;
    }
    return NULL;
}
void pStringFree(String *dest)
{
    if (dest)
    {
        if (dest->valuestring != NULL)
        {
            global_hooks.deallocate(dest->valuestring);
            dest->valuestring = NULL;
        }
        SkeletonDeallocate(dest);
    }
}
uint8_t *StringInit(String *deststring, char *valuestring, size_t valuelength)
{
    deststring->valuelength = 0;
    deststring->valuestring = NULL;
    if (valuestring && valuelength > 0)
    {
        if ((deststring->valuestring = global_hooks.allocate(valuelength + 1)))
        {
            memset(deststring->valuestring, 0x00, valuelength + 1);
            memcpy(deststring->valuestring, valuestring, valuelength);
            deststring->valuelength = valuelength;
            return deststring->valuestring;
        }
    }
    return NULL;
}
uint8_t * StringGetting(String deststring){
    return deststring.valuestring;
}

void StringFree(String deststring)
{
    if (deststring.valuestring)
        global_hooks.deallocate(deststring.valuestring);
    deststring.valuelength = 0;
    deststring.valuestring = NULL;
}

char *BufferInit(char **dest, char *datastring, size_t datalength)
{
    if ((*dest) = global_hooks.allocate(datalength + 1))
    {
        memset((*dest), 0x00, datalength + 1);
        memcpy((*dest), datastring, datalength);
        return (*dest);
    }
    return NULL;
}

void BufferFree(char *dest)
{
    if (dest)
    {
        global_hooks.deallocate(dest);
    }
}

char *BufferFormat(char **__dest, size_t fmtsize, char *format, ...)
{
    va_list args;
    va_start(args, format);
    char buffer[fmtsize];
    memset(buffer,0x00,fmtsize);
    int retsize = vsprintf(buffer, format, args);
    BufferInit(__dest,buffer,retsize);
    va_end(args);
    return (*__dest);
}

char *BufferCatenate(char *__restrict__ __dest, size_t fmtsize,char *format, ...)
{
    va_list args;
    va_start(args, format);
    char buffer[fmtsize];
    memset(buffer,0x00,fmtsize);
    int retsize = vsprintf(buffer, format, args);
    strcat(__dest,buffer);
    va_end(args);
    return __dest;
}

uint8_t *IntegerInit(uint8_t **skeleton, long value)
{
    return (*skeleton = (uint8_t *)value);
}

uint8_t *BooleanInit(uint8_t **skeleton, Boolean value)
{
    return (*skeleton = (uint8_t *)value);
}


