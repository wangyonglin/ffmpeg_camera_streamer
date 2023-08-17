#if !defined(INCLUDE_LINUXOS_CONFIG_H)
#define INCLUDE_LINUXOS_CONFIG_H
#include <linuxos/types.h>
#include <linuxos/string.h>

#include <linuxos/log.h>


typedef struct _linuxos_config_t
{

    linuxos_log_t *log;
    linuxos_string_t conf;
    linuxos_err_t err;
} linuxos_config_t, *plinuxos_config_t;

#define LINUXOS_COMMAND_STRING(name,addr) {name,LINUXOS_OBJECT_STRING,addr}
#define LINUXOS_COMMAND_INTEGER(name,addr) {name,LINUXOS_OBJECT_INTEGER,addr}
#define LINUXOS_COMMAND_BOOLEAN(name,addr) {name,LINUXOS_OBJECT_BOOLEAN,addr}
#define LINUXOS_COMMAND_NULL {NULL,-1,0}

typedef struct _linuxos_command_t
{
    const linuxos_char_t *name;
    linuxos_object_type_t type;
    linuxos_integer_t addr;
} linuxos_command_t, *plinuxos_command_t;

linuxos_config_t *linuxos_create(int argc, char *argv[]);
void linuxos_add_event(event_callback_fn callback, void *callback_arg);
void linuxos_delete(linuxos_config_t *objulinuxos);

linuxos_err_t linuxos_mapping(linuxos_config_t *config, linuxos_command_t commands[], linuxos_void_t *objectaddr, const char *section);
#endif
