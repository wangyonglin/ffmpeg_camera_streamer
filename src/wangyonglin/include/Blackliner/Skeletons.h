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


typedef struct String
{
    size_t valuelength;
    uint8_t *valuestring;
} String, *PString,DataString;
#define STRING_NULL \
    (String) { 0, NULL }
    
typedef int Exception;
#define OkException ((Exception)(0))
#define NullPointerException ((Exception)(-1))
#define RuntimeExceion ((Exception)(-2))
#define ArithmeticException ((Exception)(-3))
#define IllegalArgumentException ((Exception)(-4))
#endif