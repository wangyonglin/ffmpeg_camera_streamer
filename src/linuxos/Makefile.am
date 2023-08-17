AUTOMAKE_OPTIONS=foreign
AM_LDFLAGS= -lpthread
AM_CPPFLAGS=    \
    -I $(top_srcdir)/src/linuxos/include/

lib_LTLIBRARIES=liblinuxos.la
liblinuxos_la_SOURCES=   \
    src/linuxos/config.c  \
	src/linuxos/string.c	\
	src/linuxos/log.c	\
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

sbin_PROGRAMS     = cronolog
cronolog_SOURCES  =	\
	src/cronolog/getopt.c	\
	src/cronolog/getopt1.c	\
	src/cronolog/cronolog.c	\
	src/cronolog/cronoutils.c

noinst_PROGRAMS   = cronotest
cronotest_SOURCES = \
	src/cronolog/getopt.c		\
	src/cronolog/getopt1.c		\
	src/cronolog/cronotest.c	\
	src/cronolog/cronoutils.c

#需要引用的动态库文件
EXTRA_DIST =  $(top_srcdir)/src/linuxos/include/*
LIBS +=  -g -O0 -lm
