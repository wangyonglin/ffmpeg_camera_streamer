#if !defined(INCLUDE_BLACKLINER_LOGGER_H)
#define INCLUDE_BLACKLINER_LOGGER_H
#include <Blackliner/Skeletons.h>
#include <zlog/zlog.h>
#include <Blackliner/Configuring.h>
typedef struct logger
{
    zlog_category_t *log;
    Boolean display_errors;
    String rule_error;
    String rule_info;
    String simple;
    String normal;

} logger;


#define logger_info(cat, ...) zlog_info(cat->log,__VA_ARGS__)
#define logger_error(cat, ...)   zlog_error(cat->log,__VA_ARGS__)
logger *logger_init(logger **__logger, Configuring *config);
void logger_destroy(logger *_logger);
#endif