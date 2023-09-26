AUTOMAKE_OPTIONS=foreign
AM_LDFLAGS= -lpthread
LIBS += -g -O2 -Wall -pedantic   -lm
AM_CPPFLAGS= -I $(top_srcdir)/src/wangyonglin/include/
lib_LTLIBRARIES=libwangyonglin.la
libwangyonglin_la_SOURCES=\
        src/Blackliner/Skeletons.c \
        src/Blackliner/String.c \
        src/Blackliner/EventTrigger.c \
        src/Blackliner/KVTailq.c \
        src/Blackliner/Unix_timestamp_converter.c \
        src/Blackliner/Configuring.c \
		src/Blackliner/Logger.c \
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
        src/HTTPSClient/hostcheck.c \
        src/zlog/buf.c \
		src/zlog/conf.c \
		src/zlog/lockfile.c \
		src/zlog/record.c \
		src/zlog/rule.c \
		src/zlog/zc_arraylist.c \
		src/zlog/zc_util.c \
		src/zlog/category.c \
		src/zlog/event.c \
		src/zlog/level.c \
		src/zlog/record_table.c \
		src/zlog/spec.c \
		src/zlog/zc_hashtable.c \
		src/zlog/zlog.c \
		src/zlog/category_table.c\
		src/zlog/format.c \
		src/zlog/level_list.c \
		src/zlog/mdc.c \
		src/zlog/rotater.c \
		src/zlog/thread.c \
		src/zlog/zc_profile.c \
		src/zlog/zlog-chk-conf.c

        