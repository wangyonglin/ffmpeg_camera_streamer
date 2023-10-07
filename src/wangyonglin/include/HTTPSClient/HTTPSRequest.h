/*

  Configuring *config = NULL;
  ConfiguringInit(&config, inif);

  logger *log = NULL;
  logger_init(&log, config);
  HTTPSResponse *response = NULL;
  HTTPSRequest *request = HTTPSRequestInit(config, log);
  if (request)
  {
    HTTPSRequesting(request, "https://www.baidu.com", NULL, &response);

    logger_info(log, "wangyonglin%s", response->context);
    HTTPSResponseDeallocate(response);
    HTTPSRequestDeallocate(request);
  }

  logger_info(log, "wangyonglin%s", log->rule_error.valuestring);

  logger_destroy(log);
  ConfiguringDeallocate(config);

*/
#if !defined(INCLUDE_HTTPSCLIENT_HTTPSREQUEST_H)
#define INCLUDE_HTTPSCLIENT_HTTPSREQUEST_H
#include <Blackliner/Blackliner.h>
#include <strings.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <event2/bufferevent_ssl.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/http.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <HTTPSClient/openssl_hostname_validation.h>
#include <cJSON/cJSON.h>
#include <Blackliner/Skeletons.h>
#include <Blackliner/Logger.h>

typedef enum __HTTPSClientAction
{
  HTTPSCLIENT_REQUEST_POST = EVHTTP_REQ_POST,
  HTTPSCLIENT_REQUEST_GET = EVHTTP_REQ_GET
} HTTPSClientAction;

typedef struct HTTPSRequest
{
  Configuring *config;
  logger *logging;
  const char *url;
  char *post;
  int code;
  struct event_base *base;
  struct evhttp_uri *http_uri;
  SSL_CTX *ssl_ctx;
  SSL *ssl;
  // struct bufferevent *bev;
  struct evhttp_connection *evcon;
  struct evhttp_request *req;
  struct evkeyvalq *output_headers;
  struct evbuffer *output_buffer;
  char uri[256];
  int port;
  int retries;
  int timeout;
  const char *scheme;
  const char *host;
  const char *path;
  const char *query;
  struct bufferevent *bev;
  String response;
  HostnameValidationResult err;
  KVTailq *kvTailqHeader;
  int ignore_cert;
  int ipv6;
  int ipv4;
} HTTPSRequest;

typedef struct HTTPSResponse
{

  Configuring *config;
  logger *logging;
  struct bufferevent *bev;
  uint8_t * context;
  Integer  response_buffer_max;
} HTTPSResponse;



HTTPSRequest *HTTPSRequestInit(Configuring *config, logger *logging);
Exception HTTPSRequesting(HTTPSRequest *request, char *url, char *data, HTTPSResponse **__response);

void HTTPSRequestDeallocate(  HTTPSRequest *request);

HTTPSResponse *HTTPSResponseInit(HTTPSResponse **__response, Configuring *config, logger *logging);
void HTTPSResponseDeallocate(HTTPSResponse *response);
#endif