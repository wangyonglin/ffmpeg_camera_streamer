#if !defined(INCLUDE_FFMPEG_AVCAMERACONTROLLERS_H)
#define INCLUDE_FFMPEG_AVCAMERACONTROLLERS_H
#include <FFMPEG/AVContentSkeletons.h>

void AVCameraClear(AVInputContextSkeleton *ipt_ctx_skell);

exp_t AVCameraInit(AVInputContextSkeleton **__ipt_ctx_skell,
                   const char *device_index,
                   int width,
                   int height,
                   int fps);

#endif
