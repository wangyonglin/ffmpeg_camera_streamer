#include <Blackliner/Configuring.h>


Configuring *ConfiguringInit(Configuring **config, char *filename)
{
    assert(filename);
    Configuring *__config = NULL;
    if (((__config) = SkeletonAllocate((void **)config, sizeof(Configuring))))
    {
        if (!BufferInit(&__config->filename, filename, strlen(filename)))
        {
            SkeletonDeallocate(__config);
            return NULL;
        }
        __config->conf = NCONF_new(NULL);
        if (__config->conf == NULL)
        {
            SkeletonDeallocate(__config);
            return NULL;
        }
        __config->bio = BIO_new_file(__config->filename, "r");
        if (__config->bio == NULL)
        {
            NCONF_free(__config->conf);
            SkeletonDeallocate(__config);
            return NULL;
        }
        NCONF_load_bio(__config->conf, __config->bio, &__config->lELine);
    }
    return __config;
}

void ConfiguringDeallocate(Configuring *config)
{
    if (config)
    {
        if (config->bio)
        {
            BIO_free(config->bio);
        }
        if (config->conf)
        {
            NCONF_free(config->conf);
        }
        SkeletonDeallocate(config);
    }
}

ok_t ConfiguringString(Configuring *config, String *skeleton, const char *section, const char *name, uint8_t *value)
{
    assert(name);
    if (config && skeleton)
    {
        char *out = NCONF_get_string(config->conf, section, name);
        if (out)
        {
            StringInit((String *)skeleton, out, strlen(out));
            return bl_ok;
        }
        else
        {
            if (value)
            {
                int valuelenth = strlen(value);
                StringInit((String *)skeleton, value, valuelenth);
                return bl_ok;
            }
            else
            {
                StringInit((String *)skeleton, NULL, 0);
            }

            return bl_err;
        }
    }
    return bl_fail;
}
ok_t ConfiguringInteger(Configuring *config, Integer *skeleton, const char *section, const char *name,Integer value)
{
    assert(name);
    if (config && skeleton)
    {
        long callintger;
        if (NCONF_get_number(config->conf, section, name, &callintger) == 1)
        {
            IntegerInit((uint8_t **)skeleton, callintger);
            return bl_ok;
        }else{
             IntegerInit((uint8_t **)skeleton, value);
             return bl_err;
        }
    }
    return bl_fail;
}
ok_t ConfiguringBoolean(Configuring *config, Boolean *skeleton, const char *section, const char *name,Boolean value)
{
    assert(name);
    if (config && skeleton)
    {
        char *buffer = NCONF_get_string(config->conf, section, name);
        if (buffer)
        {

            if ((strcmp(buffer, "on") == 0) || (strcmp(buffer, "ON") == 0) || strcmp(buffer, "On") == 0)
            {

                BooleanInit((uint8_t **)skeleton, Boolean_true);
                return ok_success;
            }
            else if ((strcmp(buffer, "off") == 0) || (strcmp(buffer, "OFF") == 0) || (strcmp(buffer, "Off") == 0))
            {
                BooleanInit((uint8_t **)skeleton, Boolean_false);
                return ok_success;
            }
        }
        else
        {
            BooleanInit((uint8_t **)skeleton, value);
            return ok_error;
        }
    }
    return ok_fail;
}