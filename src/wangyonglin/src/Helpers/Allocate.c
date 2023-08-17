#include <Helpers/Allocate.h>

void *AllocateHelpers(void **dest, size_t destsize)
{

    if ((*dest) = malloc(destsize))
    {
        return memset((*dest), 0x00, destsize);
    }
}

void DeallocateHelpers(void *dest)
{
    if (dest)
    {
        free(dest);
        dest = NULL;
    }
}
