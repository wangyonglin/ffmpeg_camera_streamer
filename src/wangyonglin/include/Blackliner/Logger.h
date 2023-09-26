#if !defined(INCLUDE_BLACKLINER_LOGGER_H)
#define INCLUDE_BLACKLINER_LOGGER_H
#include <Blackliner/Skeletons.h>
#include <zlog/zlog.h>
#include <Blackliner/Configuring.h>
typedef struct logger
{
    zlog_category_t *log;
    Boolean display_errors;
    String log_error;
    String log_rule_error;
    String log_rule_info;
    String name;
    String simple;
    String normal;
    Integer port;
} logger;

//void logger_error(logger *_logger, const char *format, ...) __attribute__((format(printf, 2, 3)));

#define logger_info(cat, ...) zlog_info(cat->log,__VA_ARGS__)
#define logger_error(cat, ...)   zlog_error(cat->log,__VA_ARGS__)
logger *logger_init(logger **__logger, Configuring *config);
void logger_destroy(logger *_logger);
#endif