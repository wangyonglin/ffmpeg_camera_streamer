#include <FFMPEG/AVCameraSkeletons.h>
#include <FFMPEG/AVRTMPSkeletons.h>
#include <FFMPEG/AVFilterSkeletons.h>

// const char *filter_descr = "scale=78:24,transpose=cclock";
//  const char *filter_descr = "lutyuv='u=128:v=128'";
// const char *filter_descr = "boxblur";
// const char *filter_descr = "hflip";
// const char *filter_descr = "hue='h=60:s=-3'";
// const char *filter_descr = "crop=2/3*in_w:2/3*in_h";
// const char *filter_descr = "drawbox=x=100:y=100:w=100:h=100:color=pink@0.5";
const char *filter_descr = "drawtext=fontfile=arial.ttf:fontcolor=red:fontsize=60:text='WangYonglin'";

#define OUTPUT_URL "rtmp://192.168.1.2:1935/live/livestream"
AVOutputContextSkeleton *opt_ctx_skell = NULL;
AVInputContextSkeleton *ipt_ctx_skell = NULL;
AVFilterSkeleton *filter_skell = NULL;
bl_t loop = positive;
void handle_signal(int signal)
{
  fprintf(stderr, "Caught SIGINT, exiting now...\n");
  loop = negative;
}

int main(int argc, char *argv[])
{

  signal(SIGINT, handle_signal);
  fprintf(stdout, "\tffmpeg_camera_streamer ...\r\n");

  AVContentInit();
  AVCameraInit(&ipt_ctx_skell, "/dev/video0", 800, 600, 25);

  AVRTMPInit(&opt_ctx_skell, OUTPUT_URL, 800, 600, 25);
  AVFilterInit(&filter_skell, ipt_ctx_skell, opt_ctx_skell, filter_descr);

  AVRTMPFilterWriteFrame(opt_ctx_skell, filter_skell, &loop);
  AVFilterClear(filter_skell);

  AVRTMPClear(opt_ctx_skell);

  AVCameraClear(ipt_ctx_skell);
  return 0;
}