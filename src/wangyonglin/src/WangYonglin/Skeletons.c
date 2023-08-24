#include <WangYonglin/Skeletons.h>

void *SkeletonAllocate(void **dest, size_t destsize)
{

    if ((*dest) = malloc(destsize))
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

char *StringAllocate(char **dest, char *datastring, size_t datalength)
{
    if ((*dest) = malloc(datalength + 1))
    {
        memset((*dest), 0x00, datalength + 1);
        memcpy((*dest), datastring, datalength);
        return (*dest);
    }
    return NULL;
}

void StringDeallocate(char *dest)
{
    if (dest)
    {
        free(dest);
    }
}
