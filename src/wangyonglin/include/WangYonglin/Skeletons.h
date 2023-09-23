#if !defined(INCLUDE_WANGYONGLIN_SKELETONS_H)
#define INCLUDE_WANGYONGLIN_SKELETONS_H

#include <sys/queue.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <event2/bufferevent_ssl.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/http.h>
#include <openssl/conf.h>
#include <openssl/bio.h>

typedef int bl_t;
#define negative ((bl_t)(0))
#define positive ((bl_t)(1))

typedef int exp_t;

#define exp_ok ((exp_t)(0))
#define exp_fail ((exp_t)(-1))
#define exp_nullpointer ((exp_t)(-2))





typedef enum __class_type_t
{
    class_string_type,
    class_boolean_type,
    class_integer_type
} class_type_t;

void *SkeletonAllocate(void **dest, size_t destsize);
void SkeletonDeallocate(void *dest);

char *StringAllocate(char **dest, char *datastring, size_t datalength);
void StringDeallocate(char *dest);
#endif