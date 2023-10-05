#if !defined(INCLUDE_BLACKLINER_SKELETONS_H)
#define INCLUDE_BLACKLINER_SKELETONS_H

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

#ifndef INTERNAL_HOOKS_THIS
#define INTERNAL_HOOKS_THIS
typedef struct internal_hooks
{
    void *(*allocate)(size_t objsize);
    void (*deallocate)(void *obj);
    void *(*reallocate)(void *obj, size_t objsize);

} internal_hooks;
static internal_hooks global_hooks = {malloc, free, realloc};
#endif

typedef int ok_t;
#define bl_ok ((ok_t)(0))
#define bl_fail ((ok_t)(-1))
#define bl_err ((ok_t)(-2))
#define ok_success ((ok_t)(0))
#define ok_fail ((ok_t)(-1))
#define ok_error ((ok_t)(-2))

typedef long Integer;
typedef int Boolean;
#define bl_true ((Boolean)(1))
#define bl_false ((Boolean)(0))
#define bl_invalid ((Boolean)(-1))
#define Boolean_true ((Boolean)(1))
#define Boolean_false ((Boolean)(0))
#define Boolean_invalid ((Boolean)(-1))
typedef int bl_t;
#define negative ((bl_t)(0))
#define positive ((bl_t)(1))

typedef int exp_t;

#define exp_ok ((exp_t)(0))
#define exp_fail ((exp_t)(-1))
#define exp_nullpointer ((exp_t)(-2))

typedef enum
{
    SKELETON_TYPE_FILE,
    SKELETON_TYPE_STRING,
    SKELETON_TYPE_INTEGER,
    SKELETON_TYPE_BOOLEAN,
    SKELETON_TYPE_INVALID
} Skeletons;

typedef enum __class_type_t
{
    class_string_type,
    class_boolean_type,
    class_integer_type
} class_type_t;

typedef struct String
{
    size_t valuelength;
    uint8_t *valuestring;
} String, *PString;
#define STRING_NULL \
    (String) { 0, NULL }

void *SkeletonAllocate(void **dest, size_t destsize);
void SkeletonDeallocate(void *dest);

String *pStringInit(String **deststring, char *valuestring, size_t valuelength);
void pStringFree(String *dest);

uint8_t *StringInit(String *deststring, char *valuestring, size_t valuelength);
uint8_t * StringGetting(String deststring);
void StringFree(String deststring);

void *SkeletonInit(void **dest, size_t destsize);
void SkeletonFree(void *dest);

char *BufferInit(char **dest, char *datastring, size_t datalength);
void BufferFree(char *dest);
char *BufferFormat(char **__dest, size_t fmtsize, char *format, ...);
char *BufferCatenate(char *__restrict__ __dest, size_t fmtsize, char *format, ...);

uint8_t *IntegerInit(uint8_t **skeleton, long value);
uint8_t *BooleanInit(uint8_t **skeleton, Boolean value);
#endif