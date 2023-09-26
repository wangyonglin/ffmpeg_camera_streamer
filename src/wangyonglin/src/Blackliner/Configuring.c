#include <Blackliner/Configuring.h>

// Autowired *AutowiredInit(Autowired **__autowired, char *filename)
// {
//     assert(filename);
//     Autowired *obj = NULL;
//     if (((obj) = SkeletonAllocate((void **)__autowired, sizeof(Autowired))))
//     {
//         if (!BufferInit(&obj->filename, filename, strlen(filename)))
//         {
//             SkeletonDeallocate(obj);
//             return NULL;
//         }
//         obj->conf = NCONF_new(NULL);
//         if (obj->conf == NULL)
//         {
//             SkeletonDeallocate(obj);
//             return NULL;
//         }
//         obj->bio = BIO_new_file(obj->filename, "r");
//         if (obj->bio == NULL)
//         {
//             NCONF_free(obj->conf);
//             SkeletonDeallocate(obj);
//             return NULL;
//         }
//         NCONF_load_bio(obj->conf, obj->bio, &obj->lELine);
//     }
//     return obj;
// }

// void AutowiredFree(Autowired *autowired)
// {
//     if (autowired)
//     {
//         if (autowired->bio)
//         {
//             BIO_free(autowired->bio);
//         }
//         if (autowired->conf)
//         {
//             NCONF_free(autowired->conf);
//         }
//         SkeletonDeallocate(autowired);
//     }
// }

// void AutowiredLoader(Autowired *autowired, AutowiredCommand command[], void *skeleton, const char *section)
// {

//     if (autowired)
//     {
//         int i = 0;

//         while (command[i].name != NULL)
//         {
//             command[i].addr += (size_t)(skeleton);

//             if (command[i].skell == SKELETON_TYPE_STRING)
//             {

//                 char *out = NCONF_get_string(autowired->conf, section, command[i].name);
//                 if (out)
//                 {
//                     StringInit((String *)command[i].addr, out, strlen(out));
//                 }
//                 else
//                 {
//                     StringInit((String *)command[i].addr, NULL, 0);
//                 }
//             }
//             else if (command[i].skell == SKELETON_TYPE_INTEGER)
//             {
//                 long callintger;
//                 if (NCONF_get_number(autowired->conf, section, command[i].name, &callintger) == 1)
//                 {
//                     IntegerInit((uint8_t **)command[i].addr, callintger);
//                     //   fprintf(stdout, "\t[%s=>%ld]\r\n", command[i].name, callintger);
//                 }
//                 else
//                 {

//                     //   fprintf(stderr, "\t[%s=>%ld]\r\n", command[i].name, NULL);
//                 }
//             }
//             else if (command[i].skell == SKELETON_TYPE_BOOLEAN)
//             {
//                 char *buffer = NCONF_get_string(autowired->conf, section, command[i].name);
//                 if (buffer)
//                 {

//                     if ((strcmp(buffer, "on") == 0) || (strcmp(buffer, "ON") == 0) || strcmp(buffer, "On") == 0)
//                     {

//                         BooleanInit((uint8_t **)command[i].addr, Boolean_true);
//                     }
//                     else if ((strcmp(buffer, "off") == 0) || (strcmp(buffer, "OFF") == 0) || (strcmp(buffer, "Off") == 0))
//                     {
//                         BooleanInit((uint8_t **)command[i].addr, Boolean_false);
//                     }
//                 }
//                 else
//                 {
//                     BooleanInit((uint8_t **)command[i].addr, Boolean_invalid);
//                 }
//             }

//             i++;
//         }
//     }
// }

// void StringAutowired(String *skeleton, Autowired *autowired, const char *section, const char *name)
// {
//     if (autowired)
//     {

//         // addr += skeleton;

//         char *out = NCONF_get_string(autowired->conf, section, name);
//         if (out)
//         {
//             StringInit((String *)skeleton, out, strlen(out));
//             fprintf(stdout, "\t[section=>%s][name=>%s][value=>%s]\r\n", section, name, out);
//         }
//         else
//         {
//             StringInit((String *)skeleton, NULL, 0);
//         }
//     }
// }

Configuring *ConfiguringInit(Configuring **config, char *filename)
{
    assert(filename);
    Configuring *__config = NULL;
    if (((__config) = SkeletonInit((void **)config, sizeof(Configuring))))
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

void ConfiguringDestroy(Configuring *config)
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

ok_t ConfiguringString(Configuring *config, String *skeleton, const char *section, const char *name)
{
    assert(name);
    if (config && skeleton)
    {
        char *out = NCONF_get_string(config->conf, section, name);
        if (out)
        {
            StringInit((String *)skeleton, out, strlen(out));
            return ok_success;
        }
        else
        {
            StringInit((String *)skeleton, NULL, 0);
            return ok_error;
        }
    }
    return ok_fail;
}
ok_t ConfiguringInteger(Configuring *config, Integer *skeleton, const char *section, const char *name)
{
    assert(name);
    if (config && skeleton)
    {
        long callintger;
        if (NCONF_get_number(config->conf, section, name, &callintger) == 1)
        {
            IntegerInit((uint8_t **)skeleton, callintger);
            return ok_success;
        }
    }
    return ok_fail;
}
ok_t ConfiguringBoolean(Configuring *config, Boolean *skeleton, const char *section, const char *name)
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
            BooleanInit((uint8_t **)skeleton, Boolean_invalid);
            return ok_error;
        }
    }
    return ok_fail;
}