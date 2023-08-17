#if !defined(INCLUDE_LINUXOS_LOG_H)
#define INCLUDE_LINUXOS_LOG_H
#include <zlog/zlog.h>

#include <linuxos/string.h>
#define linuxos_log_info(cat,_VA_ARGS__...) zlog(cat, __FILE__, sizeof(__FILE__)-1, __func__, sizeof(__func__)-1, __LINE__, ZLOG_LEVEL_INFO, _VA_ARGS__)
#define linuxos_log_error(cat,_VA_ARGS__...) zlog(cat, __FILE__, sizeof(__FILE__)-1, __func__, sizeof(__func__)-1, __LINE__, ZLOG_LEVEL_ERROR, _VA_ARGS__)
typedef struct _linuxos_log_t
{
    zlog_category_t * zlog;
    linuxos_err_t err;
}linuxos_log_t;

linuxos_log_t * linuxos_log_create();

void linuxos_log_delete(linuxos_log_t *log);


#endif