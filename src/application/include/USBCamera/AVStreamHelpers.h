#if !defined(INCLUDE_USBCAMERA_AVSTREAMHELPERS_H)
#define INCLUDE_USBCAMERA_AVSTREAMHELPERS_H

#include <Helpers/String.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include <math.h>

#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libavutil/timestamp.h>
#include <libavutil/avassert.h>
#include <libavutil/channel_layout.h>
#include <libavutil/opt.h>
#include <libavutil/mathematics.h>
#include <libavutil/timestamp.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>

// a wrapper around a single output AVStream
typedef struct __AVStreamHelpers_t
{
    AVStream *st;
    AVCodecContext *enc;

    /* pts of the next frame that will be generated */
    int64_t next_pts;
    int samples_count;

    AVFrame *frame;
    AVFrame *tmp_frame;

    float t, tincr, tincr2;

    struct SwsContext *sws_ctx;
    struct SwrContext *swr_ctx;
} AVStreamHelpers;

void log_packet(const AVFormatContext *fmt_ctx, const AVPacket *pkt);
AVFrame *alloc_picture(enum AVPixelFormat pix_fmt, int width, int height);

int write_frame(AVFormatContext *fmt_ctx, const AVRational *time_base, AVStream *st, AVPacket *pkt);
/* Add an output stream. */
void add_stream(AVStreamHelpers *ost, AVFormatContext *oc,
                       AVCodec **codec,
                       enum AVCodecID codec_id);
#endif
