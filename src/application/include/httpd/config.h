#if !defined(INCLUDE_HTTPD_CONFIG_H)
#define INCLUDE_HTTPD_CONFIG_H
#include <linuxos/config.h>
#include <event2/http.h>
#define HTTPD_ROUTE_COMMAND(path, cb,args) \
    {                                 \
        path, cb, args                \
    }

#define HTTPD_ROUTE_COMMAND_NULL \
    {                            \
        NULL, NULL, NULL         \
    }

typedef struct _httpd_route_command_t
{
    const char *path;
    void (*cb)(struct evhttp_request *, void *);
    void *cb_arg;
} httpd_route_command_t;

typedef struct _httpd_config_t
{
    struct evhttp *http_server;
    struct evhttp_bound_socket * bound_socket;
    linuxos_config_t *config;
    linuxos_string_t address;
    linuxos_integer_t port;
    linuxos_err_t err;
} httpd_config_t;

void httpd_route(httpd_config_t *httpd, httpd_route_command_t command[]);
httpd_config_t *httpd_create(linuxos_config_t *config);
void httpd_delete(httpd_config_t *httpd);
linuxos_err_t httpd_start(httpd_config_t *httpd);

#endif
