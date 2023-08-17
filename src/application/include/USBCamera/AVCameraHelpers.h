#if !defined(INCLUDE_USBCAMERA_AVCAMERAHELPERS_H)
#define INCLUDE_USBCAMERA_AVCAMERAHELPERS_H

#include <Helpers/String.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libavutil/timestamp.h>

typedef struct __AVCameraHelpers
{
    int width;
    int height;
    int fps;
    AVInputFormat *ipt_fmt;
    AVFormatContext *fmt_ctx;
    AVCodec *codec;
    AVStream *stream;
    AVCodecContext *codec_ctx;
} AVCameraHelpers;

Exception AVCameraInit(
    AVCameraHelpers **ipt_ctx,
    const char *short_name,
    const char *device_index,
    int width,
    int height,
    int fps);

void AVCameraDestroy(
    AVCameraHelpers *ipt_ctx);

#endif
