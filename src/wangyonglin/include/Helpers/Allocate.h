#if !defined(INCLUDE_HELPERS_ALLOCATE_H)
#define INCLUDE_HELPERS_ALLOCATE_H

#include <sys/queue.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <event2/bufferevent_ssl.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/http.h>
#include <openssl/conf.h>
#include <openssl/bio.h>


void *AllocateHelpers(void **dest, size_t destsize);
void DeallocateHelpers(void *dest);

#endif