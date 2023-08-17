#include <linuxos/string.h>

char *linuxos_string_allocate(linuxos_string_t *dest_string, char *valuestring, size_t valuelength)
{
    if (dest_string->valuestring = linuxos_global_hooks.allocate(valuelength + 1))
    {
        memset(dest_string->valuestring, 0x00, valuelength + 1);
        if (valuestring)
            memcpy(dest_string->valuestring, valuestring, valuelength);
        dest_string->valuelength = valuelength;
        return dest_string->valuestring;
    }
    return NULL;
}
void linuxos_string_deallocate(linuxos_string_t dest_string)
{
    if (dest_string.valuestring)
    {
        linuxos_global_hooks.deallocate(dest_string.valuestring);
        dest_string.valuestring = NULL;
        dest_string.valuelength = 0;
    }
}

linuxos_boolean_t linuxos_string_empty(linuxos_string_t dest_string)
{
    linuxos_boolean_t err = bfalse;

    if (dest_string.valuestring && dest_string.valuelength == 0)
    {
        if (strcmp(dest_string.valuestring, "") != 0)
        {
            return btrue;
        }
    }

    return err;
}

void *linuxos_allocate(void **dest_object, size_t dest_size)
{
    if ((*dest_object = linuxos_global_hooks.allocate(dest_size)))
    {
        memset(*dest_object, 0x00, dest_size);
        return *dest_object;
    }
    return *dest_object = NULL;
}
/**
 *
 */
void linuxos_deallocate(void *objthis)
{
    if (objthis)
    {
        linuxos_global_hooks.deallocate(objthis);
        objthis = NULL;
    }
}

void linuxos_pboolean_setting(linuxos_uint8_t **pointeraddr, linuxos_boolean_t value)
{
    *pointeraddr = (linuxos_uint8_t *)value;
}

void linuxos_pinteger_setting(linuxos_uint8_t **pointeraddr, linuxos_integer_t value)
{
    *pointeraddr = (linuxos_uint8_t *)value;
}

linuxos_boolean_t linuxos_pstring_setting(linuxos_uint8_t **pointeraddr, linuxos_char_t *valuestring)
{
    linuxos_boolean_t err_return = bfalse;
    linuxos_string_t *dist_string = (linuxos_string_t *)(*pointeraddr);
    size_t datalength = strlen(valuestring);
    if ((dist_string->valuestring) = linuxos_global_hooks.allocate(datalength + 1))
    {
        memset(dist_string->valuestring, 0x00, datalength + 1);
        memcpy(dist_string->valuestring, valuestring, datalength);
        dist_string->valuelength = datalength;
        err_return = btrue;
    }
    return err_return;
}