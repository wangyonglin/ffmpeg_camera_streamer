#if !defined(INCLUDE_USBCAMERA_LIVESTREAMER_H)
#define INCLUDE_USBCAMERA_LIVESTREAMER_H

#include <Helpers/String.h>
#include <USBCamera/AVCameraHelpers.h>
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

typedef struct __AVOutputHelpers
{
    int width;
    int height;
    int fps;
    AVPacket *pkt;
    AVFrame *frame;
    AVFrame *outframe;
    AVFormatContext *fmt_ctx;
    AVCodec *codec;
    AVStream *stream;
    AVCodecContext *codec_ctx;
} AVOutputHelpers;

exp_t AVOutputInit(
    AVOutputHelpers **opt_ctx,
    const char *output_path,
    const char *output_format,
    int width,
    int height,
    int fps);

void AVOutputDestroy(
    AVOutputHelpers *opt_ctx);
#endif
