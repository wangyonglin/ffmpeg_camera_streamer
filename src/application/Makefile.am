AUTOMAKE_OPTIONS=foreign
AM_CPPFLAGS=\
    -I $(top_srcdir)/src/linuxos/include/  \
    -I $(top_srcdir)/src/application/include/  \
    -I $(top_srcdir)/src/wangyonglin/include/

sbin_PROGRAMS=ffmpeg_camera_streamer
ffmpeg_camera_streamer_SOURCES=\
    src/main.c  \
    src/httpd/httpd.c \
    src/FFMPEG/AVRTMPSkeletons.c \
    src/FFMPEG/AVCameraSkeletons.c \
    src/FFMPEG/AVContentSkeletons.c \
    src/FFMPEG/AVFilterSkeletons.c


    

ffmpeg_camera_streamer_LDADD=\
    $(top_builddir)/src/linuxos/liblinuxos.la\
    $(top_builddir)/src/wangyonglin/libwangyonglin.la
    $(FLIBS) 
EXTRA_DIST	 =  $(top_srcdir)/src/application/include/*
#需要引用的动态库文件
LIBS += -g -O2 -Wall -pedantic   -lm -levent -lssl -lcrypto -lpthread -levent_openssl -lcurl -lavformat -lavcodec -lavutil -lavdevice -lswscale -lswresample -lavfilter
