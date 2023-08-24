#include <FFMPEG/AVCameraSkeletons.h>

void AVCameraClear(AVContextSkeleton *ipt_ctx_skell)
{
    if (ipt_ctx_skell == NULL)
        return;
    if (ipt_ctx_skell->fmt_ctx != NULL)
    {
        avformat_free_context(ipt_ctx_skell->fmt_ctx);
        fprintf(stderr, "avformat_free_context.ipt_ctx_skell\n");
        if (ipt_ctx_skell->fmt_ctx->pb != NULL)
        {
            avio_close(ipt_ctx_skell->fmt_ctx->pb);
            fprintf(stderr, "avio_close.ipt_ctx_skell\n");
        }
    }

    SkeletonDeallocate(ipt_ctx_skell);
}

exp_t AVCameraInit(AVInputContextSkeleton **__ipt_ctx_skell,
                   const char *device_index,
                   int width,
                   int height,
                   int fps)
{

    AVInputContextSkeleton *ipt_ctx_skell = NULL;

    if (!(ipt_ctx_skell = ( AVInputContextSkeleton *)SkeletonAllocate((void **)__ipt_ctx_skell, sizeof(AVInputContextSkeleton))))
    {
        return exp_nullpointer;
    }

    if (!(ipt_ctx_skell->ipt_fmt = av_find_input_format("v4l2")))
    {
        fprintf(stderr, "av_find_input_format(\"%s\")\n", "v4l2");
        return exp_fail;
    }
    if (avformat_open_input(&(ipt_ctx_skell->fmt_ctx), device_index, ipt_ctx_skell->ipt_fmt, NULL) != 0)
    {
        fprintf(stderr, "cannot initialize input device!\n");
        return exp_fail;
    }
    avformat_find_stream_info(ipt_ctx_skell->fmt_ctx, 0);

    ipt_ctx_skell->codec = avcodec_find_decoder(ipt_ctx_skell->fmt_ctx->streams[0]->codecpar->codec_id);
    ipt_ctx_skell->stream = avformat_new_stream(ipt_ctx_skell->fmt_ctx, ipt_ctx_skell->codec);
    ipt_ctx_skell->codec_ctx = avcodec_alloc_context3(ipt_ctx_skell->codec);

    AVDictionary *codec_options = NULL;
    char str_framerate[8] = {0};
    char str_video_size[16] = {0};
    sprintf(str_framerate, "%d", fps);
    sprintf(str_video_size, "%dx%d", width, height);
    av_dict_set(&codec_options, "video_size", str_video_size, 0);
    av_dict_set(&codec_options, "framerate", str_framerate, 0);
    av_dict_set(&codec_options, "preset", "superfast", 0);
    av_dict_set(&codec_options, "pixel_format", "yuyv422", 0);
    av_dict_set(&codec_options, "probesize", "7000000", 0);
    avcodec_parameters_to_context(ipt_ctx_skell->codec_ctx, ipt_ctx_skell->fmt_ctx->streams[0]->codecpar);
    if (avcodec_open2(ipt_ctx_skell->codec_ctx, ipt_ctx_skell->codec, &codec_options) != 0)
    {
        fprintf(stderr, "cannot initialize video decoder!\n");
         return exp_fail;
    }
    return exp_ok;
}
