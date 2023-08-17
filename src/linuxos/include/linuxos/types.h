#if !defined(INCLUDE_LINUXOS_TYPES_H)
#define INCLUDE_LINUXOS_TYPES_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <event2/event.h>
#include <event2/event_compat.h>

typedef int linuxos_err_t;
typedef void linuxos_void_t;
typedef char linuxos_char_t;
typedef long linuxos_boolean_t;
typedef long linuxos_integer_t;
typedef unsigned long linuxos_size_t;

typedef uint8_t linuxos_uint8_t;
typedef uint16_t linuxos_uint16_t;

#define err_ok ((linuxos_err_t)0)
#define err_fail ((linuxos_err_t)-1)
// #define NullpointerException ((linuxos_err_t)-2)
// #define ArgumentException ((linuxos_err_t)-3)
// #define AccessException ((linuxos_err_t)-4)
// #define ArithmeticException ((linuxos_err_t)-5)
// #define ObjectInitException ((linuxos_err_t)-5)

#define btrue ((linuxos_boolean_t)1)
#define bfalse ((linuxos_boolean_t)0)
#define binvalid ((linuxos_boolean_t)-1)

typedef struct _linuxos_string_t
{
    linuxos_integer_t valuelength;
    linuxos_uint8_t *valuestring;
} linuxos_string_t, *plinuxos_string_t;

typedef struct _linuxos_internal_hooks_t
{
    void *(*allocate)(size_t objsize);
    void (*deallocate)(void *obj);
    void *(*reallocate)(void *obj, size_t objsize);

} linuxos_internal_hooks_t;

static linuxos_internal_hooks_t linuxos_global_hooks = {malloc, free, realloc};
typedef enum _linuxos_object_type_t
{
    LINUXOS_OBJECT_STRING,
    LINUXOS_OBJECT_BOOLEAN,
    LINUXOS_OBJECT_INTEGER
} linuxos_object_type_t;
#endif
