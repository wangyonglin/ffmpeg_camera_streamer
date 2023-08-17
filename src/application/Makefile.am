AUTOMAKE_OPTIONS=foreign
AM_CPPFLAGS=\
    -I $(top_srcdir)/src/linuxos/include/  \
    -I $(top_srcdir)/src/application/include/  \
    -I $(top_srcdir)/src/wangyonglin/include/

sbin_PROGRAMS=wangyonglind
wangyonglind_SOURCES=\
    src/main.c  \
    src/httpd/httpd.c \
    src/USBCamera/AVOutputHelpers.c \
    src/USBCamera/AVCameraHelpers.c \
    src/USBCamera/AVEffectHelpers.c \
    src/USBCamera/AVManagerHelpers.c \
    src/USBCamera/AVStreamHelpers.c

    

wangyonglind_LDADD=\
    $(top_builddir)/src/linuxos/liblinuxos.la\
    $(top_builddir)/src/wangyonglin/libwangyonglin.la
    $(FLIBS) 
EXTRA_DIST	 =  $(top_srcdir)/src/application/include/*
#需要引用的动态库文件
LIBS += -g -O2 -Wall -pedantic   -lm -levent -lssl -lcrypto -lpthread -levent_openssl -lcurl -lavformat -lavcodec -lavutil -lavdevice -lswscale
