// #if defined(__APPLE__) && defined(__clang__)
// #pragma clang diagnostic ignored "-Wdeprecated-declarations"
// #endif
// #include <HTTPSClient/HTTPSClient.h>

// Exception HTTPSClientRequest(HTTPSClient *client, unsigned char *ppostData);

// HTTPSClient *HTTPSClientCreate(const char *url)
// {
// 	HTTPSClient *clinet = NULL;
// 	if (SkeletonInit((void **)&clinet, sizeof(HTTPSClient)))
// 	{
// 		clinet->url = url;
// 		clinet->base = NULL;
// 		clinet->http_uri = NULL;
// 		clinet->retries = 0;
// 		clinet->timeout - 1;
// 		clinet->kvTailqHeader = KVTailqCreate();
// 		memset(clinet->uri, 0x00, sizeof(clinet->uri));
// 		return clinet;
// 	}
// 	return clinet;
// }

// Exception HTTPSClientAddHeader(HTTPSClient *client, const char *name, char *value)
// {
// 	if (!client)
// 		return IllegalArgumentException;
// 	KVTailqAdd(client->kvTailqHeader, name, value);
// 	return OkException;
// }

// void HTTPSClientDestroy(HTTPSClient *pHTTPSClient)
// {
// 	if (pHTTPSClient)
// 	{
// 		KVTailqDestroy(pHTTPSClient->kvTailqHeader);
// 		SkeletonDeallocate(pHTTPSClient);
// 		StringDeallocate(pHTTPSClient->response);
// 	}
// }

// static int ignore_cert = 0;
// static int ipv6 = 0;
// static int ipv4 = 0;

// static void http_request_done(struct evhttp_request *req, void *ctx)
// {
// 	char buffer[256] = {0};
// 	int nread;

// 	if (!req || !evhttp_request_get_response_code(req))
// 	{
// 		/* If req is NULL, it means an error occurred, but
// 		 * sadly we are mostly left guessing what the error
// 		 * might have been.  We'll do our best... */

// 		struct bufferevent *bev = ((HTTPSClient *)ctx)->bev;

// 		unsigned long oslerr;
// 		int printed_err = 0;
// 		int errcode = EVUTIL_SOCKET_ERROR();
// 		fprintf(stderr, "some request failed - no idea which one though!\n");
// 		/* Print out the OpenSSL error queue that libevent
// 		 * squirreled away for us, if any. */
// 		while ((oslerr = bufferevent_get_openssl_error(bev)))
// 		{
// 			ERR_error_string_n(oslerr, buffer, sizeof(buffer));
// 			fprintf(stderr, "%s\n", buffer);
// 			printed_err = 1;
// 		}
// 		/* If the OpenSSL error queue was empty, maybe it was a
// 		 * socket error; let's try printing that. */
// 		if (!printed_err)
// 			fprintf(stderr, "socket error = %s (%d)\n",
// 					evutil_socket_error_to_string(errcode),
// 					errcode);

// 		return;
// 	}

// 	fprintf(stderr, "Response line: %d %s\n",
// 			evhttp_request_get_response_code(req),
// 			evhttp_request_get_response_code_line(req));

// 	((HTTPSClient *)ctx)->code = evhttp_request_get_response_code(req);
// 	while ((nread = evbuffer_remove(evhttp_request_get_input_buffer(req),
// 									buffer, sizeof(buffer))) > 0)
// 	{
// 		/* These are just arbitrary chunks of 256 bytes.
// 		 * They are not lines, so we can't treat them as such. */
// 		fwrite(buffer, nread, 1, stdout);
// 	}

// 	if (buffer)
// 	{
// 		StringInit(&((HTTPSClient *)ctx)->response, buffer, strlen(buffer));
// 	}
// 	StringInit(&((HTTPSClient *)ctx)->response, NULL, 0);
// }

// static void
// err(const char *msg)
// {
// 	fputs(msg, stderr);
// }

// static void
// err_openssl(const char *func)
// {
// 	fprintf(stderr, "%s failed:\n", func);

// 	/* This is the OpenSSL function that prints the contents of the
// 	 * error stack to the specified file handle. */
// 	ERR_print_errors_fp(stderr);

// 	exit(1);
// }

// /* See http://archives.seul.org/libevent/users/Jan-2013/msg00039.html */
// static int cert_verify_callback(X509_STORE_CTX *x509_ctx, void *arg)
// {
// 	char cert_str[256];
// 	// const char *host = (const char *)arg;
// 	HTTPSClient *client = (HTTPSClient *)arg;
// 	const char *res_str = "X509_verify_cert failed";
// 	// HostnameValidationResult res = Error;
// 	client->err = Error;
// 	/* This is the function that OpenSSL would call if we hadn't called
// 	 * SSL_CTX_set_cert_verify_callback().  Therefore, we are "wrapping"
// 	 * the default functionality, rather than replacing it. */
// 	int ok_so_far = 0;

// 	X509 *server_cert = NULL;

// 	if (ignore_cert)
// 	{
// 		return 1;
// 	}

// 	ok_so_far = X509_verify_cert(x509_ctx);

// 	server_cert = X509_STORE_CTX_get_current_cert(x509_ctx);

// 	if (ok_so_far)
// 	{
// 		client->err = validate_hostname(client->host, server_cert);

// 		switch (client->err)
// 		{
// 		case MatchFound:
// 			res_str = "MatchFound";
// 			break;
// 		case MatchNotFound:
// 			res_str = "MatchNotFound";
// 			break;
// 		case NoSANPresent:
// 			res_str = "NoSANPresent";
// 			break;
// 		case MalformedCertificate:
// 			res_str = "MalformedCertificate";
// 			break;
// 		case Error:
// 			res_str = "Error";
// 			break;
// 		default:
// 			res_str = "WTF!";
// 			break;
// 		}
// 	}

// 	X509_NAME_oneline(X509_get_subject_name(server_cert),
// 					  cert_str, sizeof(cert_str));

// 	if (client->err == MatchFound)
// 	{
// 		printf("https server '%s' has this certificate, "
// 			   "which looks good to me:\n%s\n",
// 			   client->host, cert_str);
// 		return 1;
// 	}
// 	else
// 	{
// 		printf("Got '%s' for hostname '%s' and certificate:\n%s\n",
// 			   res_str, client->host, cert_str);
// 		return 0;
// 	}
// }

// Exception HTTPSClientRequest(HTTPSClient *client, unsigned char *ppostData)
// {
// 	if (!client)
// 		return IllegalArgumentException;
// 	//	int r;
// 	const char *crt = NULL;
// 	enum
// 	{
// 		HTTP,
// 		HTTPS
// 	} type = HTTP;

// 	client->http_uri = evhttp_uri_parse(client->url);
// 	if (client->http_uri == NULL)
// 	{
// 		err("malformed url\n");
// 		goto error;
// 	}

// 	client->scheme = evhttp_uri_get_scheme(client->http_uri);
// 	if (client->scheme == NULL || (strcasecmp(client->scheme, "https") != 0 &&
// 								   strcasecmp(client->scheme, "http") != 0))
// 	{
// 		err("url must be http or https\n");
// 		goto error;
// 	}

// 	client->host = evhttp_uri_get_host(client->http_uri);
// 	if (client->host == NULL)
// 	{
// 		err("url must have a host\n");
// 		goto error;
// 	}

// 	client->port = evhttp_uri_get_port(client->http_uri);
// 	if (client->port == -1)
// 	{
// 		client->port = (strcasecmp(client->scheme, "http") == 0) ? 80 : 443;
// 	}

// 	client->path = evhttp_uri_get_path(client->http_uri);
// 	if (strlen(client->path) == 0)
// 	{
// 		client->path = "/";
// 	}

// 	client->query = evhttp_uri_get_query(client->http_uri);
// 	if (client->query == NULL)
// 	{
// 		snprintf(client->uri, sizeof(client->uri) - 1, "%s", client->path);
// 	}
// 	else
// 	{
// 		snprintf(client->uri, sizeof(client->uri) - 1, "%s?%s", client->path, client->query);
// 	}
// 	client->uri[sizeof(client->uri) - 1] = '\0';

// #if (OPENSSL_VERSION_NUMBER < 0x10100000L) || \
// 	(defined(LIBRESSL_VERSION_NUMBER) && LIBRESSL_VERSION_NUMBER < 0x20700000L)
// 	// Initialize OpenSSL
// 	SSL_library_init();
// 	ERR_load_crypto_strings();
// 	SSL_load_error_strings();
// 	OpenSSL_add_all_algorithms();
// #endif

// 	int ret = RAND_poll();
// 	if (ret == 0)
// 	{
// 		err_openssl("RAND_poll");
// 		goto error;
// 	}
// 	client->ssl_ctx = SSL_CTX_new(SSLv23_method());
// 	if (!client->ssl_ctx)
// 	{
// 		err_openssl("SSL_CTX_new");
// 		goto error;
// 	}

// 	if (crt == NULL)
// 	{
// 		X509_STORE *store;
// 		store = SSL_CTX_get_cert_store(client->ssl_ctx);
// 		if (X509_STORE_set_default_paths(store) != 1)
// 		{
// 			err_openssl("X509_STORE_set_default_paths");
// 			goto error;
// 		}
// 	}
// 	else
// 	{
// 		if (SSL_CTX_load_verify_locations(client->ssl_ctx, crt, NULL) != 1)
// 		{
// 			err_openssl("SSL_CTX_load_verify_locations");
// 			goto error;
// 		}
// 	}
// 	SSL_CTX_set_verify(client->ssl_ctx, SSL_VERIFY_PEER, NULL);
// 	SSL_CTX_set_cert_verify_callback(client->ssl_ctx, cert_verify_callback,
// 									 (void *)client);

// 	client->base = event_base_new();
// 	if (!client->base)
// 	{
// 		perror("event_base_new()");
// 		goto error;
// 	}

// 	// Create OpenSSL bufferevent and stack evhttp on top of it
// 	client->ssl = SSL_new(client->ssl_ctx);
// 	if (client->ssl == NULL)
// 	{
// 		err_openssl("SSL_new()");
// 		goto error;
// 	}

// #ifdef SSL_CTRL_SET_TLSEXT_HOSTNAME
// 	// Set hostname for SNI extension
// 	SSL_set_tlsext_host_name(client->ssl, client->host);
// #endif

// 	if (strcasecmp(client->scheme, "http") == 0)
// 	{
// 		client->bev = bufferevent_socket_new(client->base, -1, BEV_OPT_CLOSE_ON_FREE);
// 	}
// 	else
// 	{

// 		type = HTTPS;
// 		client->bev = bufferevent_openssl_socket_new(

// 			client->base, -1, client->ssl,
// 			BUFFEREVENT_SSL_CONNECTING,
// 			BEV_OPT_CLOSE_ON_FREE | BEV_OPT_DEFER_CALLBACKS);
// 	}

// 	if (client->bev == NULL)
// 	{
// 		fprintf(stderr, "bufferevent_openssl_socket_new() failed\n");
// 		goto error;
// 	}

// 	bufferevent_openssl_set_allow_dirty_shutdown(client->bev, 1);

// 	// For simplicity, we let DNS resolution block. Everything else should be
// 	// asynchronous though.
// 	{
// 		if (client->host[0] == '[' && strlen(client->host) > 2 && ipv6)
// 		{
// 			// trim '[' and ']'
// 			char *host_ipv6 = strndup(&client->host[1], strlen(&client->host[1]) - 1);
// 			client->evcon = evhttp_connection_base_bufferevent_new(client->base, NULL, client->bev,
// 																   host_ipv6, client->port);
// 			free(host_ipv6);
// 		}
// 		else
// 		{
// 			client->evcon = evhttp_connection_base_bufferevent_new(client->base, NULL, client->bev,
// 																   client->host, client->port);
// 		}
// 	}
// 	if (client->evcon == NULL)
// 	{
// 		fprintf(stderr, "evhttp_connection_base_bufferevent_new() failed\n");
// 		goto error;
// 	}

// 	if (ipv4)
// 	{
// 		evhttp_connection_set_family(client->evcon, AF_INET);
// 	}
// 	if (ipv6)
// 	{
// 		evhttp_connection_set_family(client->evcon, AF_INET6);
// 	}

// 	if (client->retries > 0)
// 	{
// 		evhttp_connection_set_retries(client->evcon, client->retries);
// 	}
// 	if (client->timeout >= 0)
// 	{
// 		evhttp_connection_set_timeout(client->evcon, client->timeout);
// 	}

// 	// Fire off the request
// 	client->req = evhttp_request_new(http_request_done, client);
// 	if (client->req == NULL)
// 	{
// 		fprintf(stderr, "evhttp_request_new() failed\n");
// 		goto error;
// 	}

// 	client->output_headers = evhttp_request_get_output_headers(client->req);
// 	evhttp_add_header(client->output_headers, "Host", client->host);
// 	evhttp_add_header(client->output_headers, "User-Agent", "PostmanRuntime/7.32.2");
// 	evhttp_add_header(client->output_headers, "Accept", "application/json");
// 	evhttp_add_header(client->output_headers, "Content-type", "application/json");
// 	evhttp_add_header(client->output_headers, "Connection", "close");

// 	char buf[1024];
// 	bzero(&buf, sizeof(buf));
// 	if (ppostData)
// 	{
// 		size_t len_postData = strlen(ppostData);
// 		client->output_buffer = evhttp_request_get_output_buffer(client->req);
// 		evbuffer_add(client->output_buffer, ppostData, len_postData);
// 		evutil_snprintf(buf, sizeof(buf) - 1, "%lu", (unsigned long)len_postData);
// 		evhttp_add_header(client->output_headers, "Content-Length", buf);
// 	}

// 	struct __kv_tailq_entry_t *current_item;
// 	TAILQ_FOREACH(current_item, &client->kvTailqHeader->head, entries)
// 	{
// 		fprintf(stdout, "\t%s=>%s\r\n", current_item->key, current_item->value);
// 		evhttp_add_header(client->output_headers, current_item->key, current_item->value);
// 	}
// 	ret = evhttp_make_request(client->evcon, client->req, ppostData ? EVHTTP_REQ_POST : EVHTTP_REQ_GET, client->uri);
// 	if (ret != 0)
// 	{
// 		fprintf(stderr, "evhttp_make_request() failed\n");
// 		goto error;
// 	}

// 	event_base_dispatch(client->base);
// 	goto cleanup;

// error:
// 	ret = 1;
// cleanup:
// 	if (client->evcon)
// 		evhttp_connection_free(client->evcon);
// 	if (client->http_uri)
// 		evhttp_uri_free(client->http_uri);
// 	if (client->base)
// 		event_base_free(client->base);

// 	if (client->ssl_ctx)
// 		SSL_CTX_free(client->ssl_ctx);
// 	if (type == HTTP && client->ssl)
// 		SSL_free(client->ssl);
// #if (OPENSSL_VERSION_NUMBER < 0x10100000L) || \
// 	(defined(LIBRESSL_VERSION_NUMBER) && LIBRESSL_VERSION_NUMBER < 0x20700000L)
// 	EVP_cleanup();
// 	ERR_free_strings();

// #if OPENSSL_VERSION_NUMBER < 0x10000000L
// 	ERR_remove_state(0);
// #else
// 	ERR_remove_thread_state(NULL);
// #endif

// 	CRYPTO_cleanup_all_ex_data();

// 	sk_SSL_COMP_free(SSL_COMP_get_compression_methods());
// #endif /* (OPENSSL_VERSION_NUMBER < 0x10100000L) || \
// 	(defined(LIBRESSL_VERSION_NUMBER) && LIBRESSL_VERSION_NUMBER < 0x20700000L) */

// 	return OkException;
// }

// Exception HTTPSClientGET(HTTPSClient *pHTTPSClient)
// {
// 	return HTTPSClientRequest(pHTTPSClient, NULL);
// }
// Exception HTTPSClientPOST(HTTPSClient *pHTTPSClient, unsigned char *ppostData)
// {
// 	return HTTPSClientRequest(pHTTPSClient, ppostData);
// }

