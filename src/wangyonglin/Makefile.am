AUTOMAKE_OPTIONS=foreign
AM_LDFLAGS= -lpthread
LIBS += -g -O2 -Wall -pedantic   -lm
AM_CPPFLAGS= -I $(top_srcdir)/src/wangyonglin/include/
lib_LTLIBRARIES=libwangyonglin.la
libwangyonglin_la_SOURCES=\
        src/Helpers/Allocate.c \
        src/Helpers/String.c \
        src/Helpers/EventTrigger.c \
        src/Helpers/KVTailq.c \
        src/Helpers/Unix_timestamp_converter.c \
        src/OpenSSL/base64.c   \
        src/SnowFlake/IdGenerator.c  \
        src/SnowFlake/IdGenOptions.c  \
        src/SnowFlake/SnowFlake.c  \
        src/SnowFlake/SnowWorkerM1.c  \
        src/SnowFlake/SnowWorkerM2.c	\
	src/cJSON/cJSON.c \
        src/cJSON/cJSON_Utils.c \
        src/HTTPSClient/HTTPSClient.c \
        src/HTTPSClient/openssl_hostname_validation.c \
        src/HTTPSClient/hostcheck.c