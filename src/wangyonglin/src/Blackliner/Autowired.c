#include <Blackliner/Autowired.h>

Autowired *AutowiredCreate(char *filename)
{
    Autowired *obj = NULL;
    assert(filename);
    if (SkeletonAllocate((void **)&obj, sizeof(Autowired)))
    {
        if (!StringInit(&obj->filename, filename, strlen(filename)))
        {
            SkeletonDeallocate(obj);
            return NULL;
        }
        obj->conf = NCONF_new(NULL);
        if (obj->conf == NULL)
        {
            SkeletonDeallocate(obj);
            return NULL;
        }
        obj->bio = BIO_new_file(obj->filename, "r");
        if (obj->bio == NULL)
        {
            NCONF_free(obj->conf);
            SkeletonDeallocate(obj);
            return NULL;
        }
        NCONF_load_bio(obj->conf, obj->bio, &obj->lELine);
    }
    return obj;
}

void AutowiredDelete(Autowired *autowired)
{
    if (autowired)
    {
        if (autowired->bio)
        {
            BIO_free(autowired->bio);
        }
        if (autowired->conf)
        {
            NCONF_free(autowired->conf);
        }
        SkeletonDeallocate(autowired);
    }
}

void AutowiredLoader(Autowired *autowired, AutowiredCommand command[], void *skeleton, const char *section)
{

    if (autowired)
    {
        int i = 0;

        while (command[i].name != NULL)
        {
            command[i].addr += (skeleton);

            if (command[i].skell == SKELETON_TYPE_STRING)
            {

                char *out = NCONF_get_string(autowired->conf, section, command[i].name);
                if (out)
                {
                    StringInit((char *)command[i].addr, out, strlen(out));
                }
                else
                {
                    command[i].addr = NULL;
                }
            }
            else if (command[i].skell == SKELETON_TYPE_INTEGER)
            {
                long callintger;
                if (NCONF_get_number(autowired->conf, section, command[i].name, &callintger) == 1)
                {
                    IntegerInit(command[i].addr, callintger);
                 //   fprintf(stdout, "\t[%s=>%ld]\r\n", command[i].name, callintger);
                }
                else
                {

                 //   fprintf(stderr, "\t[%s=>%ld]\r\n", command[i].name, NULL);
                }
            }
            else if (command[i].skell == SKELETON_TYPE_BOOLEAN)
            {
                char *buffer = NCONF_get_string(autowired->conf, section, command[i].name);
                if (buffer)
                {

                    if ((strcmp(buffer, "on") == 0) || (strcmp(buffer, "ON") == 0) || strcmp(buffer, "On") == 0)
                    {

                        BooleanInit(command[i].addr, Boolean_true);
                    }
                    else if ((strcmp(buffer, "off") == 0) || (strcmp(buffer, "OFF") == 0) || (strcmp(buffer, "Off") == 0))
                    {
                        BooleanInit(command[i].addr, Boolean_false);
                    }
                }
                else
                {
                    BooleanInit(command[i].addr, Boolean_invalid);
                }
            }

            i++;
        }
    }
}
