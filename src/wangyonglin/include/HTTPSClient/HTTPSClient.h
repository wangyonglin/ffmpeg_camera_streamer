/*
  cJSON *root = cJSON_CreateObject();
  cJSON_AddStringToObject(root, "appid", "tt5aa62c1bc59afb6f01");
  cJSON_AddStringToObject(root, "secret", "c8013c23181c59de0ae50e2c740bc13a37226746");
  cJSON_AddStringToObject(root, "grant_type", "client_credential");
  unsigned char *out = cJSON_PrintUnformatted(root);
  fprintf(stdout, "\t%s\r\n", out);
  HTTPSClient *client = HTTPSClientCreate("https://developer.toutiao.com/api/apps/v2/token");
  HTTPSClientPOST(client,out);
  fprintf(stdout, "\t%s [%d]\r\n", client->response.data, client->code);
  HTTPSClientDestroy(client);
  free(out);
  */
#if !defined(INCLUDE_HTTPSCLIENT_HTTPSCLIENT_H)
#define INCLUDE_HTTPSCLIENT_HTTPSCLIENT_H
#include <Helpers/String.h>
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
#include <Helpers/KVTailq.h>
typedef enum __HTTPSClientAction
{
  HTTPSCLIENT_REQUEST_POST = EVHTTP_REQ_POST,
  HTTPSCLIENT_REQUEST_GET = EVHTTP_REQ_GET
} HTTPSClientAction;

typedef struct __HTTPSClient
{
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
  DataString response;
  HostnameValidationResult err;
  KVTailq *kvTailqHeader;
} HTTPSClient;

HTTPSClient *HTTPSClientCreate(const char *url);
Exception HTTPSClientAddHeader(HTTPSClient *client, const char *name, char *value);
void HTTPSClientDestroy(HTTPSClient *pHTTPSClient);

Exception HTTPSClientGET(HTTPSClient *pHTTPSClient);
Exception HTTPSClientPOST(HTTPSClient *pHTTPSClient, unsigned char *ppostData);
#endif