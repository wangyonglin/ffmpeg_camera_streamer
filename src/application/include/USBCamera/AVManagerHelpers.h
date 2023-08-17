#if !defined(INCLUDE_USBCAMERA_AVMANAGERHELPERS_H)
#define INCLUDE_USBCAMERA_AVMANAGERHELPERS_H

#include <USBCamera/AVCameraHelpers.h>
#include <USBCamera/AVOutputHelpers.h>

typedef struct __AVManagerHelpers
{
    bool loop;
    AVCameraHelpers *ipt_ctx;
    AVOutputHelpers *opt_ctx;
} AVManagerHelpers;

static void log_packet(
    const AVFormatContext *fmt_ctx,
    const AVPacket *pkt);

void AVManagerDestroy(
    AVManagerHelpers *manager);
AVManagerHelpers *AVManagerInit(AVManagerHelpers **manager);

exp_t AVManagerRuning(
    AVManagerHelpers *manage,
    AVCameraHelpers *ipt_ctx,
    AVOutputHelpers *opt_ctx);

#endif
