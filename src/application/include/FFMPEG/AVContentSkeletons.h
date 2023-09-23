#if !defined(INCLUDE_FFMPEG_AVCONTENTSKELETONS_H)
#define INCLUDE_FFMPEG_AVCONTENTSKELETONS_H

#include <Blackliner/Blackliner.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include <libavfilter/avfilter.h>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libavutil/timestamp.h>
#include <libavfilter/buffersink.h>
#include <libavfilter/buffersrc.h>
#include <libavutil/opt.h>



typedef struct AVContextSkeleton
{
    AVInputFormat *ipt_fmt;
    AVFormatContext *fmt_ctx;
    AVCodec *codec;
    AVStream *stream;
    AVCodecContext *codec_ctx;
    int stream_inx;
} AVContextSkeleton;

typedef AVContextSkeleton AVInputContextSkeleton;
typedef AVContextSkeleton AVOutputContextSkeleton;

typedef struct AVStreamSkeleton
{
    int width;
    int height;
    enum AVPixelFormat pix_fmt;
    AVFrame *inframe;

    struct SwsContext *img_convert_ctx;
} AVStreamSkeleton;

void AVContentInit();
void log_packet(const AVFormatContext *fmt_ctx, const AVPacket *pkt);



#endif
