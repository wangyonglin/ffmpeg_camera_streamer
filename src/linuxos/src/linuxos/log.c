#include <linuxos/log.h>
linuxos_log_t *linuxos_log_create()
{
    linuxos_log_t *log = NULL;
    if (!linuxos_allocate((void **)&log, sizeof(linuxos_log_t)))
    {
        perror("申请日记对象错误");
        return log;
    }
    char zlog_init_values[512] = {0};
    strcat(zlog_init_values, "[global]\n");
    strcat(zlog_init_values, "strict init = true\n");
    strcat(zlog_init_values, "buffer min = 1024\n");
    strcat(zlog_init_values, "buffer max = 2048\n");
    strcat(zlog_init_values, "rotate lock file = /tmp/zlog.lock\n");
    strcat(zlog_init_values, "file perms = 600\n");
    strcat(zlog_init_values, "[formats]\n");
    strcat(zlog_init_values, "simple	= \"%d.%-8.8us %-5V [%-8.8p.%-8.8t %F %L] %m%n\"\n");
    strcat(zlog_init_values, "normal = \"%d [%V] %m%n\"\n");
    strcat(zlog_init_values, "[rules]\n");
    strcat(zlog_init_values, "zlog_rules.*		>stderr; normal\n");

    if (zlog_init_from_string(zlog_init_values))
    {
        perror("日记配置初始化失败");
        log->err = err_fail;
    }

    if (!(log->zlog = zlog_get_category("zlog_rules")))
    {
        perror("日记配置类别失败");
        zlog_fini();
        log->err = err_fail;
    }
    if (log->err != err_ok)
    {
        linuxos_deallocate(log);
    }
    return log;
}

void linuxos_log_delete(linuxos_log_t *log)
{
    if (log && log->err == err_ok)
        zlog_fini();
}
