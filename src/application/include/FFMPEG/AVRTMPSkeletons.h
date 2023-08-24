#if !defined(INCLUDE_FFMPEG_AVRTMPSKELETONS_H)
#define INCLUDE_FFMPEG_AVRTMPSKELETONS_H

#include <FFMPEG/AVContentSkeletons.h>
#include <FFMPEG/AVFilterSkeletons.h>


void AVRTMPClear(AVOutputContextSkeleton *opt_ctx_skell);

exp_t AVRTMPInit(AVOutputContextSkeleton **__opt_ctx_skell, const char *output_path,
                 int width,
                 int height,
                 int fps);


exp_t AVRTMPWriteFrame(AVOutputContextSkeleton *opt_ctx_skell, AVInputContextSkeleton *ipt_ctx_skell, bl_t *loop);
exp_t AVRTMPFilterWriteFrame(AVOutputContextSkeleton *opt_ctx_skell, AVFilterSkeleton *filter_skell, bl_t *loop);
#endif
