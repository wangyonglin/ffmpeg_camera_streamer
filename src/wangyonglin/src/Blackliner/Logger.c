#include <Blackliner/Logger.h>

logger *logger_init(logger **__logger, Configuring *config)
{
    if (!config)
        return NULL;

    logger *_logger = NULL;
    if ((_logger = SkeletonAllocate((void **)__logger, sizeof(logger))))
    {
        ConfiguringString(config, &_logger->simple, "global", "simple",NULL);
        ConfiguringString(config, &_logger->normal, "global", "normal",NULL);
        ConfiguringString(config, &_logger->rule_error, "global", "rule_error","./log/error.log");
        ConfiguringString(config, &_logger->rule_info, "global", "rule_info","./log/info.log");
        ConfiguringBoolean(config, &_logger->display_errors, "global", "display_errors",bl_true);
        // 初始化 zlog_init 字符串值
        char zlog_init_values[512] = {0};
        strcat(zlog_init_values, "[global]\n");
        strcat(zlog_init_values, "strict init = true\n");
        strcat(zlog_init_values, "buffer min = 1024\n");
        strcat(zlog_init_values, "buffer max = 2048\n");
        strcat(zlog_init_values, "rotate lock file = /tmp/zlog.lock\n");
        strcat(zlog_init_values, "file perms = 600\n");
        strcat(zlog_init_values, "[formats]\n");
        if (_logger->simple.valuestring != NULL)
        {
            BufferCatenate(zlog_init_values, 256, "simple	= \"%s\"\n", _logger->simple.valuestring);
        }
        if (_logger->normal.valuestring != NULL)
        {
            BufferCatenate(zlog_init_values, 256, "normal	= \"%s\"\n", _logger->normal.valuestring);
        }
        strcat(zlog_init_values, "[rules]\n");
        if (_logger->display_errors == Boolean_true)
        {
            BufferCatenate(zlog_init_values, 256, "log_rule.ERROR		>stderr; simple\n");
            BufferCatenate(zlog_init_values, 256, "log_rule.INFO		>stdout; normal\n");
        }
        else
        {
            if (_logger->rule_info.valuestring != NULL)
            {
                BufferCatenate(zlog_init_values, 256, "log_rule.INFO		%s\n", _logger->rule_info.valuestring);
            }
            if (_logger->rule_error.valuestring != NULL)
            {
                BufferCatenate(zlog_init_values, 256, "log_rule.ERROR		%s\n", _logger->rule_error.valuestring);
            }
        }

        if (zlog_init_from_string(zlog_init_values) != 0)
        {
            fprintf(stderr, "zlog_init failed \r\n%s\r\n", zlog_init_values);
            SkeletonDeallocate(_logger);
            return _logger = NULL;
        }

        if (!((_logger->log) = zlog_get_category("log_rule")))
        {
            fprintf(stderr, "zlog_get_category [zlog_rules] failed\n");
            zlog_fini();
            SkeletonDeallocate(_logger);
            return NULL;
        }

        return _logger;
    }
    return NULL;
}

void logger_destroy(logger *_logger)
{
    if (_logger)
    {
        zlog_fini();
        SkeletonDeallocate(_logger);
    }
}
