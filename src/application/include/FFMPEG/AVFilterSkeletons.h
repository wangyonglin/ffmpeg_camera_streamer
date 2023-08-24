#if !defined(INCLUDE_FFMPEG_AVFILTERSKELETON_H)
#define INCLUDE_FFMPEG_AVFILTERSKELETON_H

#include <FFMPEG/AVCameraSkeletons.h>

// typedef struct AVFilteringSkeleton
// {
//     AVFilterContext *buffersink_ctx;
//     AVFilterContext *buffersrc_ctx;
//     AVFilterGraph *filter_graph;
// } AVFilteringSkeleton;

// typedef struct AVStreamSkeleton
// {
//     AVCodecContext *dec_ctx;
//     AVCodecContext *enc_ctx;
// } AVStreamSkeleton;

typedef struct AVFilterSkeleton
{
    AVFilterContext *buffersink_ctx;
    AVFilterContext *buffersrc_ctx;
    AVFilterGraph *filter_graph;
    AVInputContextSkeleton *ipt_ctx_skell;
    AVOutputContextSkeleton *opt_ctx_skell;
} AVFilterSkeleton;



int AVFilterInit(AVFilterSkeleton **__filter_skell, AVInputContextSkeleton *ipt_ctx_skell,
                AVOutputContextSkeleton *opt_ctx_skell, const char *filter_spec);

void AVFilterClear(AVFilterSkeleton *filter_skell);
#endif
