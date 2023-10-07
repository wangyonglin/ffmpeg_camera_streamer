#include <HTTPSClient/HTTPSRequest.h>
#include <Blackliner/Logger.h>
// static int ignore_cert = 0;
// static int ipv6 = 0;
// static int ipv4 = 0;
/* See http://archives.seul.org/libevent/users/Jan-2013/msg00039.html */
static int __cert_verify_callback(X509_STORE_CTX *x509_ctx, void *arg)
{
    char cert_str[256];
    // const char *host = (const char *)arg;
    HTTPSRequest *request = (HTTPSRequest *)arg;
    const char *res_str = "X509_verify_cert failed";
    // HostnameValidationResult res = Error;
    request->err = Error;
    /* This is the function that OpenSSL would call if we hadn't called
     * SSL_CTX_set_cert_verify_callback().  Therefore, we are "wrapping"
     * the default functionality, rather than replacing it. */
    int ok_so_far = 0;

    X509 *server_cert = NULL;

    ok_so_far = X509_verify_cert(x509_ctx);

    server_cert = X509_STORE_CTX_get_current_cert(x509_ctx);

    if (ok_so_far)
    {
        request->err = validate_hostname(request->host, server_cert);

        switch (request->err)
        {
        case MatchFound:
            res_str = "MatchFound";
            break;
        case MatchNotFound:
            res_str = "MatchNotFound";
            break;
        case NoSANPresent:
            res_str = "NoSANPresent";
            break;
        case MalformedCertificate:
            res_str = "MalformedCertificate";
            break;
        case Error:
            res_str = "Error";
            break;
        default:
            res_str = "WTF!";
            break;
        }
    }

    X509_NAME_oneline(X509_get_subject_name(server_cert),
                      cert_str, sizeof(cert_str));

    if (request->err == MatchFound)
    {
        printf("https server '%s' has this certificate, "
               "which looks good to me:\n%s\n",
               request->host, cert_str);
        return 1;
    }
    else
    {
        printf("Got '%s' for hostname '%s' and certificate:\n%s\n",
               res_str, request->host, cert_str);
        return 0;
    }
}

static void __http_request_done(struct evhttp_request *req, void *ctx)
{
    char buffer[256] = {0};
    int nread;

    if (!req || !evhttp_request_get_response_code(req))
    {
        /* If req is NULL, it means an error occurred, but
         * sadly we are mostly left guessing what the error
         * might have been.  We'll do our best... */

        struct bufferevent *bev = ((HTTPSResponse *)ctx)->bev;
        logger *logging = ((HTTPSResponse *)ctx)->logging;

        unsigned long oslerr;
        int printed_err = 0;
        int errcode = EVUTIL_SOCKET_ERROR();
        fprintf(stderr, "some request failed - no idea which one though!\n");
        /* Print out the OpenSSL error queue that libevent
         * squirreled away for us, if any. */
        while ((oslerr = bufferevent_get_openssl_error(bev)))
        {
            ERR_error_string_n(oslerr, buffer, sizeof(buffer));
            fprintf(stderr, "%s\n", buffer);

            printed_err = 1;
        }
        /* If the OpenSSL error queue was empty, maybe it was a
         * socket error; let's try printing that. */
        if (!printed_err)
            fprintf(stderr, "socket error = %s (%d)\n",
                    evutil_socket_error_to_string(errcode),
                    errcode);

        return;
    }

    fprintf(stderr, "Response line: %d %s\n",
            evhttp_request_get_response_code(req),
            evhttp_request_get_response_code_line(req));

    ((HTTPSRequest *)ctx)->code = evhttp_request_get_response_code(req);
    while ((nread = evbuffer_remove(evhttp_request_get_input_buffer(req),
                                    buffer, sizeof(buffer))) > 0)
    {
        /* These are just arbitrary chunks of 256 bytes.
         * They are not lines, so we can't treat them as such. */
        fwrite(buffer, nread, 1, stdout);
    }

    if (buffer)
    {
        StringInit(&((HTTPSRequest *)ctx)->response, buffer, strlen(buffer));
    }
    StringInit(&((HTTPSRequest *)ctx)->response, NULL, 0);
}

HTTPSRequest *HTTPSRequestInit(Configuring *config, logger *logging)
{
    HTTPSRequest *request = NULL;
    if (SkeletonAllocate((void **)&request, sizeof(HTTPSRequest)))
    {
        request->config = config;
        request->logging = logging;
        request->base = NULL;
        request->http_uri = NULL;
        request->retries = 0;
        request->timeout - 1;
        request->ipv6 = 0;
        request->ipv4 = 0;
        request->ignore_cert = 0;
        request->kvTailqHeader = KVTailqCreate();

        return request;
    }
    return request;
}

Exception HTTPSRequesting(HTTPSRequest *request, char *url, char *data, HTTPSResponse **__response)
{
    assert(url);
    if (!request)
        return IllegalArgumentException;
    HTTPSResponse *response = NULL;
    response = HTTPSResponseInit(__response, request->config, request->logging);
    const char *crt = NULL;
    enum
    {
        HTTP,
        HTTPS
    } type = HTTP;

    request->http_uri = evhttp_uri_parse(url);
    if (request->http_uri == NULL)
    {
        logger_error(request->logging, "malformed url\n");
        goto error;
    }

    request->scheme = evhttp_uri_get_scheme(request->http_uri);
    if (request->scheme == NULL || (strcasecmp(request->scheme, "https") != 0 &&
                                    strcasecmp(request->scheme, "http") != 0))
    {
        logger_error(request->logging, "url must be http or https\n");
        goto error;
    }

    request->host = evhttp_uri_get_host(request->http_uri);
    if (request->host == NULL)
    {
        logger_error(request->logging, "url must have a host\n");
        goto error;
    }

    request->port = evhttp_uri_get_port(request->http_uri);
    if (request->port == -1)
    {
        request->port = (strcasecmp(request->scheme, "http") == 0) ? 80 : 443;
    }

    request->path = evhttp_uri_get_path(request->http_uri);
    if (strlen(request->path) == 0)
    {
        request->path = "/";
    }

    request->query = evhttp_uri_get_query(request->http_uri);
    if (request->query == NULL)
    {
        snprintf(request->uri, sizeof(request->uri) - 1, "%s", request->path);
    }
    else
    {
        snprintf(request->uri, sizeof(request->uri) - 1, "%s?%s", request->path, request->query);
    }
    request->uri[sizeof(request->uri) - 1] = '\0';

#if (OPENSSL_VERSION_NUMBER < 0x10100000L) || \
    (defined(LIBRESSL_VERSION_NUMBER) && LIBRESSL_VERSION_NUMBER < 0x20700000L)
    // Initialize OpenSSL
    SSL_library_init();
    ERR_load_crypto_strings();
    SSL_load_error_strings();
    OpenSSL_add_all_algorithms();
#endif

    int ret = RAND_poll();
    if (ret == 0)
    {
        logger_error(request->logging, "RAND_poll");
        goto error;
    }
    request->ssl_ctx = SSL_CTX_new(SSLv23_method());
    if (!request->ssl_ctx)
    {
        logger_error(request->logging, "SSL_CTX_new");
        goto error;
    }

    if (crt == NULL)
    {
        X509_STORE *store;
        store = SSL_CTX_get_cert_store(request->ssl_ctx);
        if (X509_STORE_set_default_paths(store) != 1)
        {
            logger_error(request->logging, "X509_STORE_set_default_paths");
            goto error;
        }
    }
    else
    {
        if (SSL_CTX_load_verify_locations(request->ssl_ctx, crt, NULL) != 1)
        {
            logger_error(request->logging, "SSL_CTX_load_verify_locations");
            goto error;
        }
    }
    SSL_CTX_set_verify(request->ssl_ctx, SSL_VERIFY_PEER, NULL);
    SSL_CTX_set_cert_verify_callback(request->ssl_ctx, __cert_verify_callback,
                                     (void *)request);

    request->base = event_base_new();
    if (!request->base)
    {
        perror("event_base_new()");
        goto error;
    }

    // Create OpenSSL bufferevent and stack evhttp on top of it
    request->ssl = SSL_new(request->ssl_ctx);
    if (request->ssl == NULL)
    {
        logger_error(request->logging, "SSL_new()");
        goto error;
    }

#ifdef SSL_CTRL_SET_TLSEXT_HOSTNAME
    // Set hostname for SNI extension
    SSL_set_tlsext_host_name(request->ssl, request->host);
#endif

    if (strcasecmp(request->scheme, "http") == 0)
    {
        response->bev = bufferevent_socket_new(request->base, -1, BEV_OPT_CLOSE_ON_FREE);
    }
    else
    {
        type = HTTPS;
        response->bev = bufferevent_openssl_socket_new(

            request->base, -1, request->ssl,
            BUFFEREVENT_SSL_CONNECTING,
            BEV_OPT_CLOSE_ON_FREE | BEV_OPT_DEFER_CALLBACKS);
    }

    if (response->bev == NULL)
    {
        logger_error(request->logging, "bufferevent_openssl_socket_new() failed\n");
        goto error;
    }

    bufferevent_openssl_set_allow_dirty_shutdown(response->bev, 1);

    // For simplicity, we let DNS resolution block. Everything else should be
    // asynchronous though.
    {
        if (request->host[0] == '[' && strlen(request->host) > 2 && request->ipv6)
        {
            // trim '[' and ']'
            char *host_ipv6 = strndup(&request->host[1], strlen(&request->host[1]) - 1);
            request->evcon = evhttp_connection_base_bufferevent_new(request->base, NULL, response->bev,
                                                                    host_ipv6, request->port);
            free(host_ipv6);
        }
        else
        {
            request->evcon = evhttp_connection_base_bufferevent_new(request->base, NULL, response->bev,
                                                                    request->host, request->port);
        }
    }
    if (request->evcon == NULL)
    {
        logger_error(request->logging, "evhttp_connection_base_bufferevent_new() failed\n");
        goto error;
    }

    if (request->ipv4)
    {
        evhttp_connection_set_family(request->evcon, AF_INET);
    }
    if (request->ipv6)
    {
        evhttp_connection_set_family(request->evcon, AF_INET6);
    }

    if (request->retries > 0)
    {
        evhttp_connection_set_retries(request->evcon, request->retries);
    }
    if (request->timeout >= 0)
    {
        evhttp_connection_set_timeout(request->evcon, request->timeout);
    }

    // Fire off the request
    request->req = evhttp_request_new(__http_request_done, response);
    if (request->req == NULL)
    {
        fprintf(stderr, "evhttp_request_new() failed\n");
        goto error;
    }

    request->output_headers = evhttp_request_get_output_headers(request->req);
    evhttp_add_header(request->output_headers, "Host", request->host);
    evhttp_add_header(request->output_headers, "User-Agent", "PostmanRuntime/7.32.2");
    evhttp_add_header(request->output_headers, "Accept", "application/json");
    evhttp_add_header(request->output_headers, "Content-type", "application/json");
    evhttp_add_header(request->output_headers, "Connection", "close");

    char buf[1024];
    bzero(&buf, sizeof(buf));
    if (data)
    {
        size_t len_postData = strlen(data);
        request->output_buffer = evhttp_request_get_output_buffer(request->req);
        evbuffer_add(request->output_buffer, data, len_postData);
        evutil_snprintf(buf, sizeof(buf) - 1, "%lu", (unsigned long)len_postData);
        evhttp_add_header(request->output_headers, "Content-Length", buf);
    }

    struct __kv_tailq_entry_t *current_item;
    TAILQ_FOREACH(current_item, &request->kvTailqHeader->head, entries)
    {
        fprintf(stdout, "\t%s=>%s\r\n", current_item->key, current_item->value);
        evhttp_add_header(request->output_headers, current_item->key, current_item->value);
    }
    ret = evhttp_make_request(request->evcon, request->req, data ? EVHTTP_REQ_POST : EVHTTP_REQ_GET, request->uri);
    if (ret != 0)
    {
        fprintf(stderr, "evhttp_make_request() failed\n");
        goto error;
    }

    event_base_dispatch(request->base);
    goto cleanup;

error:
    ret = 1;
cleanup:
    if (request->evcon)
        evhttp_connection_free(request->evcon);
    if (request->http_uri)
        evhttp_uri_free(request->http_uri);
    if (request->base)
        event_base_free(request->base);

    if (request->ssl_ctx)
        SSL_CTX_free(request->ssl_ctx);
    if (type == HTTP && request->ssl)
        SSL_free(request->ssl);
#if (OPENSSL_VERSION_NUMBER < 0x10100000L) || \
    (defined(LIBRESSL_VERSION_NUMBER) && LIBRESSL_VERSION_NUMBER < 0x20700000L)
    EVP_cleanup();
    ERR_free_strings();

#if OPENSSL_VERSION_NUMBER < 0x10000000L
    ERR_remove_state(0);
#else
    ERR_remove_thread_state(NULL);
#endif

    CRYPTO_cleanup_all_ex_data();

    sk_SSL_COMP_free(SSL_COMP_get_compression_methods());
#endif /* (OPENSSL_VERSION_NUMBER < 0x10100000L) || \
    (defined(LIBRESSL_VERSION_NUMBER) && LIBRESSL_VERSION_NUMBER < 0x20700000L) */

    return OkException;
}

void HTTPSRequestDeallocate(HTTPSRequest *request)
{
    if (request)
    {
        KVTailqDestroy(request->kvTailqHeader);
        SkeletonDeallocate(request);
        StringDeallocate(request->response);
    }
}

HTTPSResponse *HTTPSResponseInit(HTTPSResponse **__response, Configuring *config, logger *logging)
{
    HTTPSResponse *response = NULL;
    if (response = SkeletonAllocate((void **)__response, sizeof(HTTPSResponse)))
    {
        response->config = config;
        response->logging = logging;
        ConfiguringInteger(config, &response->response_buffer_max, "HTTPS", "response_buffer_max", 1024);
        BufferAllocate(&response->context, response->response_buffer_max);
        return response;
    }
    return NULL;
}

void HTTPSResponseDeallocate(HTTPSResponse *response)
{
    if (response)
    {
        BufferDeallocate(response->context);
        SkeletonDeallocate(response);
    }
}