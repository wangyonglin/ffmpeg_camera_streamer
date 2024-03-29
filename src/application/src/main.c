#include <FFMPEG/AVCameraSkeletons.h>
#include <FFMPEG/AVRTMPSkeletons.h>
#include <FFMPEG/AVFilterSkeletons.h>
#include <Blackliner/Blackliner.h>
#include <Blackliner/Logger.h>
#include <Blackliner/Configuring.h>
#include <HTTPSClient/HTTPSRequest.h>
// const char *filter_descr = "scale=78:24,transpose=cclock";
//  const char *filter_descr = "lutyuv='u=128:v=128'";
// const char *filter_descr = "boxblur";
// const char *filter_descr = "hflip";
// const char *filter_descr = "hue='h=60:s=-3'";
// const char *filter_descr = "crop=2/3*in_w:2/3*in_h";
// const char *filter_descr = "drawbox=x=100:y=100:w=100:h=100:color=pink@0.5";
const char *filter_descr = "drawtext=fontfile=arial.ttf:fontcolor=red:fontsize=60:text='WangYonglin'";

char *inif = "/home/wangyonglin/github/wangyonglin/ffmpeg_camera_streamer/conf/wangyonglin.conf";
// ddd
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

  // AVContentInit();
  // AVCameraInit(&ipt_ctx_skell, "/dev/video0", 800, 600, 25);

  // AVRTMPInit(&opt_ctx_skell, OUTPUT_URL, 800, 600, 25);
  // AVFilterInit(&filter_skell, ipt_ctx_skell, opt_ctx_skell, filter_descr);

  // AVRTMPFilterWriteFrame(opt_ctx_skell, filter_skell, &loop);

  // AVFilterClear(filter_skell);
  // AVRTMPClear(opt_ctx_skell);
  // AVCameraClear(ipt_ctx_skell);

  Configuring *config = NULL;
  ConfiguringInit(&config, inif);

  logger *log = NULL;
  logger_init(&log, config);
  HTTPSResponse *response = NULL;
  HTTPSRequest *request = HTTPSRequestInit(config, log);
  if (request)
  {
    HTTPSRequesting(request, "https://www.baidu.com", NULL, &response);

    logger_info(log, "wangyonglin%s", response->context);
    HTTPSResponseDeallocate(response);
    HTTPSRequestDeallocate(request);
  }

  logger_info(log, "wangyonglin%s", log->rule_error.valuestring);

  logger_destroy(log);
  ConfiguringDeallocate(config);

  return 0;
}