#include <linuxos/config.h>
#include <httpd/config.h>

#include <event2/http.h>
#include <event2/http_struct.h>
#include <event2/util.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/event.h>
#include <event2/http.h>
#include <event2/ws.h>

#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/queue.h>
#include <sys/stat.h>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <getopt.h>
#include <io.h>

#ifndef stat
#define stat _stat
#endif
#ifndef fstat
#define fstat _fstat
#endif
#ifndef open
#define open _open
#endif
#ifndef close
#define close _close
#endif
#ifndef O_RDONLY
#define O_RDONLY _O_RDONLY
#endif

#else /* !_WIN32 */

#ifdef EVENT__HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif
#ifdef EVENT__HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif
#ifdef EVENT__HAVE_NETINET_IN6_H
#include <netinet/in6.h>
#endif
#include <unistd.h>

#endif /* _WIN32 */

#define log_d(...) fprintf(stderr, __VA_ARGS__)

typedef struct client
{
    struct evws_connection *evws;
    char name[INET6_ADDRSTRLEN];
    TAILQ_ENTRY(client)
    next;
} client_t;

typedef TAILQ_HEAD(clients_s, client) clients_t;

static clients_t clients;

static void
broadcast_msg(char *msg)
{
    struct client *client;

    TAILQ_FOREACH(client, &clients, next)
    {
        evws_send(client->evws, msg, strlen(msg));
    }
    log_d("%s\n", msg);
}

static void
on_msg_cb(struct evws_connection *evws, int type, const unsigned char *data,
          size_t len, void *arg)
{

    struct client *self = arg;
    char buf[4096];
    const char *msg = (const char *)data;

    snprintf(buf, sizeof(buf), "%.*s", (int)len, msg);
    if (len == 5 && memcmp(buf, "/quit", 5) == 0)
    {
        evws_close(evws, WS_CR_NORMAL);
        snprintf(buf, sizeof(buf), "'%s' left the chat", self->name);
    }
    else if (len >  6 && strncmp(msg, "/name ", 6) == 0)
    {
        const char *new_name = (const char *)msg + 6;
        int name_len = len - 6;

        snprintf(buf, sizeof(buf), "'%s' renamed itself to '%.*s'", self->name,
                 name_len, new_name);
        snprintf(
            self->name, sizeof(self->name) - 1, "%.*s", name_len, new_name);
    }
    else
    {
        snprintf(buf, sizeof(buf), "[%s] %.*s", self->name, (int)len, msg);
    }

    broadcast_msg(buf);
}

static void
on_close_cb(struct evws_connection *evws, void *arg)
{
    client_t *client = arg;
    log_d("'%s' disconnected\n", client->name);
    TAILQ_REMOVE(&clients, client, next);
    free(arg);
}

static const char *
nice_addr(const char *addr)
{
    if (strncmp(addr, "::ffff:", 7) == 0)
        addr += 7;

    return addr;
}

static void
addr2str(struct sockaddr *sa, char *addr, size_t len)
{
    const char *nice;
    unsigned short port;
    size_t adlen;

    if (sa->sa_family == AF_INET)
    {
        struct sockaddr_in *s = (struct sockaddr_in *)sa;
        port = ntohs(s->sin_port);
        evutil_inet_ntop(AF_INET, &s->sin_addr, addr, len);
    }
    else
    { // AF_INET6
        struct sockaddr_in6 *s = (struct sockaddr_in6 *)sa;
        port = ntohs(s->sin6_port);
        evutil_inet_ntop(AF_INET6, &s->sin6_addr, addr, len);
        nice = nice_addr(addr);
        if (nice != addr)
        {
            size_t len = strlen(addr) - (nice - addr);
            memmove(addr, nice, len);
            addr[len] = 0;
        }
    }
    adlen = strlen(addr);
    snprintf(addr + adlen, len - adlen, ":%d", port);
}

static void on_ws(struct evhttp_request *req, void *arg)
{

    struct event_base *base = arg;
    struct client *client;
    evutil_socket_t fd;
    struct sockaddr_storage addr;
    socklen_t len;

    client = calloc(sizeof(*client), 1);

    client->evws =
        evws_new_session(req, on_msg_cb, client, 0);

    if (!client->evws)
    {
        free(client);
        log_d("Failed to create session\n");
        return;
    }

    fd = bufferevent_getfd(evws_connection_get_bufferevent(client->evws));

    len = sizeof(addr);
    getpeername(fd, (struct sockaddr *)&addr, &len);
    addr2str((struct sockaddr *)&addr, client->name, sizeof(client->name));

    log_d("New client joined from %s\n", client->name);

    evws_connection_set_closecb(client->evws, on_close_cb, client);
    TAILQ_INSERT_TAIL(&clients, client, next);
}

#ifndef EVENT__HAVE_STRSIGNAL
static inline const char *
strsignal(evutil_socket_t sig)
{
    return "Signal";
}
#endif

httpd_config_t *httpd_create(linuxos_config_t *config)
{
    if (config == NULL)
        return NULL;
    httpd_config_t *httpd = NULL;
    if (!linuxos_allocate((void **)&httpd, sizeof(httpd_config_t)))
    {
        linuxos_log_error(config->log->zlog, "HTTPD服务器 申请内存指针失败");
        return httpd;
    }

    linuxos_command_t commands[] = {
        LINUXOS_COMMAND_STRING("address", offsetof(httpd_config_t, address)),
        LINUXOS_COMMAND_INTEGER("port", offsetof(httpd_config_t, port)),
        LINUXOS_COMMAND_NULL};

    httpd->err = linuxos_mapping(config, commands, httpd, "HTTPD");
    linuxos_log_info(config->log->zlog, "%s", httpd->address.valuestring);
    linuxos_log_info(config->log->zlog, "%ld", httpd->port);
    httpd->config = config;
    return httpd;
}

void httpd_route(httpd_config_t *httpd, httpd_route_command_t command[])
{
    if (!httpd)
        return;
    signal(SIGPIPE, SIG_IGN);
    TAILQ_INIT(&clients);
    // if (!(httpd->http_server = evhttp_new(httpd->config->event->base)))
    //     linuxos_log_error(httpd->config->log->zlog, "HTTPD服务器->evhttp_new 失败");

    httpd->bound_socket = evhttp_bind_socket_with_handle(httpd->http_server, httpd->address.valuestring, httpd->port);
    if (!httpd->bound_socket)
    {
        fprintf(stderr, "Could not bind to port 8080!\n");
        return;
    }

   // evhttp_set_cb(httpd->http_server, "/ws", on_ws, httpd->config->event->base);
}
linuxos_err_t httpd_start(httpd_config_t *httpd)
{
    if (!httpd)
        return err_fail;
    signal(SIGPIPE, SIG_IGN);
    TAILQ_INIT(&clients);
    // if (!(httpd->http_server = evhttp_new(httpd->config->event->base)))
    //     linuxos_log_error(httpd->config->log->zlog, "HTTPD服务器->evhttp_new 失败");

    evhttp_bind_socket_with_handle(httpd->http_server, httpd->address.valuestring, httpd->port);
   // evhttp_set_cb(httpd->http_server, "/ws", on_ws, httpd->config->event->base);
    return err_ok;
}
void httpd_delete(httpd_config_t *httpd)
{
    if (httpd)
    {

        if (httpd->http_server)
        {
            evhttp_free(httpd->http_server);
            httpd->http_server = NULL;
        }
        linuxos_deallocate(httpd);
        httpd = NULL;
    }
}
