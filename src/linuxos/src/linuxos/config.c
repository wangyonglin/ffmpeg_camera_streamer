#include <linuxos/config.h>
#include <openssl/conf.h>
#include <openssl/bio.h>

#define LINUXOS_CONFIG_CONF "/home/wangyonglin/github/wangyonglin/wangyonglin/conf/wangyonglin.conf"

void linuxos_integer_setting(linuxos_uint8_t **pointeraddr, const CONF *conf, const char *group, const char *name);
void linuxos_string_setting(linuxos_uint8_t **pointeraddr, const CONF *conf, const char *group, const char *name);
void linuxos_boolean_setting(linuxos_uint8_t **pointeraddr, const CONF *conf, const char *group, const char *name);

linuxos_config_t *linuxos_create(int argc, char *argv[])
{
    linuxos_config_t *config = NULL;

    if (!linuxos_allocate((void **)&config, sizeof(linuxos_config_t)))
    {
        perror("主要程序启动失败,自动退出程序");
        exit(err_fail);
    }
    // 配置日记文件
    if (!(config->log = linuxos_log_create()))
    {
        config->err = err_fail;
        perror("日记启动失败");
        return config;
    }



    config->conf.valuestring = strdup(LINUXOS_CONFIG_CONF);
    config->conf.valuelength = strlen(LINUXOS_CONFIG_CONF);

    return config;
}

void linuxos_delete(linuxos_config_t *config)
{
    fprintf(stdout, "\texit\r\n");
    if (config)
    {

        linuxos_log_delete(config->log);

        linuxos_string_deallocate(config->conf);
        linuxos_deallocate(config);
    }
}

linuxos_err_t linuxos_mapping(linuxos_config_t *config, linuxos_command_t commands[], linuxos_void_t *objectaddr, const char *section)
{
    linuxos_err_t err = err_fail;
    if (!config)
    {
        return err;
    }
    CONF *pConf = NCONF_new(NULL);
    if (!pConf)
    {
        return err;
    }
    BIO *pBio = BIO_new_file(config->conf.valuestring, "r");

    if (pBio == NULL)
    {
        perror("打开配置文件失败");
        return err;
    }

    long callintger;
    long lELine = 0;
    NCONF_load_bio(pConf, pBio, &lELine);
    int i = 0;

    while (commands[i].name != NULL)
    {
        commands[i].addr += (linuxos_integer_t)(objectaddr);

        switch (commands[i].type)
        {
        case LINUXOS_OBJECT_STRING:
            linuxos_string_setting((linuxos_uint8_t **)commands[i].addr, pConf, section, commands[i].name);
            break;
        case LINUXOS_OBJECT_BOOLEAN:
            linuxos_boolean_setting((linuxos_uint8_t **)commands[i].addr, pConf, section, commands[i].name);
            break;
        case LINUXOS_OBJECT_INTEGER:
            linuxos_integer_setting((linuxos_uint8_t **)commands[i].addr, pConf, section, commands[i].name);
            break;
        default:
            break;
        }
        i++;
    }
    BIO_free(pBio);
    NCONF_free(pConf);
    return err_ok;
}

void linuxos_integer_setting(linuxos_uint8_t **pointeraddr, const CONF *conf, const char *group, const char *name)
{
    long call_integer;
    if (NCONF_get_number(conf, group, name, &call_integer) == 1)
    {
        //  if (log)
        //    zlog_info(log, "[配置文件获取成功[%s=>%s]值为{%lld}", group, commands[i].name, callintger);
        linuxos_pinteger_setting(pointeraddr, call_integer);
    }
}

void linuxos_string_setting(linuxos_uint8_t **pointeraddr, const CONF *conf, const char *group, const char *name)
{

    linuxos_char_t *resultstring = (linuxos_char_t *)NCONF_get_string(conf, group, name);
    if (resultstring)
    {
        linuxos_pstring_setting((linuxos_uint8_t **)&pointeraddr, resultstring);
    }
    else
    {
        ((linuxos_string_t *)pointeraddr)->valuestring = NULL;
        ((linuxos_string_t *)pointeraddr)->valuelength = 0;
    }
}

void linuxos_boolean_setting(linuxos_uint8_t **pointeraddr, const CONF *conf, const char *group, const char *name)
{

    linuxos_char_t *resultstring = (linuxos_char_t *)NCONF_get_string(conf, group, name);
    if (resultstring)
    {

        if ((strcmp(resultstring, "on") == 0) || (strcmp(resultstring, "ON") == 0) || strcmp(resultstring, "On") == 0)
        {
            //  if (log)
            //    zlog_info(log, "[配置文件获取成功[%s=>%s]值为{true}", section, commands[i].name);
            linuxos_pboolean_setting((linuxos_uint8_t **)pointeraddr, btrue);
        }
        else if ((strcmp(resultstring, "off") == 0) || (strcmp(resultstring, "OFF") == 0) || (strcmp(resultstring, "Off") == 0))
        {
            //  if (log)
            //      zlog_info(log, "[配置文件获取成功[%s=>%s]值为{%false}", section, commands[i].name);
            linuxos_pboolean_setting((linuxos_uint8_t **)pointeraddr, bfalse);
        }
    }
    else
    {
        linuxos_pboolean_setting(pointeraddr, binvalid);
    }
}