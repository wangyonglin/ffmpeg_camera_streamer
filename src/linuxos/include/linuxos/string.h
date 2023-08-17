#if !defined(INCLUDE_LINUXOS_STRING_H)
#define INCLUDE_LINUXOS_STRING_H
#include <linuxos/types.h>

char *linuxos_string_allocate(linuxos_string_t *dest_string, char *valuestring, size_t valuelength);
void linuxos_string_deallocate(linuxos_string_t dest_string);
linuxos_boolean_t linuxos_string_empty(linuxos_string_t dest_string);

void *linuxos_allocate(void **dest_object, size_t dest_size);
void linuxos_deallocate(void *objthis);


void linuxos_pboolean_setting(linuxos_uint8_t **pointeraddr, linuxos_boolean_t value);
void linuxos_pinteger_setting(linuxos_uint8_t **pointeraddr, linuxos_integer_t value);
linuxos_boolean_t linuxos_pstring_setting(linuxos_uint8_t **pointeraddr, linuxos_char_t *valuestring);
#endif