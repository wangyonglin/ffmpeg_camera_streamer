#include <USBCamera/AVCameraHelpers.h>

Exception AVCameraInit(
    AVCameraHelpers **ipt_ctx,
    const char *short_name,
    const char *device_index,
    int width,
    int height,
    int fps)
{

    if (!AllocateHelpers((void **)ipt_ctx, sizeof(AVCameraHelpers)))
    {
        fprintf(stderr, "AVCameraHelpers ipt_ctx\n");
        return NullPointerException;
    }

    if (!((*ipt_ctx)->ipt_fmt = av_find_input_format(short_name)))
    {
        fprintf(stderr, "av_find_input_format(\"%s\")\n", short_name);
        DeallocateHelpers(*ipt_ctx);
        (*ipt_ctx) = NULL;
        return RuntimeExceion;
    }
    if (avformat_open_input(&((*ipt_ctx)->fmt_ctx), device_index, (*ipt_ctx)->ipt_fmt, NULL) != 0)
    {
        fprintf(stderr, "cannot initialize input device!\n");
        DeallocateHelpers((*ipt_ctx));
        (*ipt_ctx) = NULL;
        return RuntimeExceion;
    }
    avformat_find_stream_info((*ipt_ctx)->fmt_ctx, 0);

    (*ipt_ctx)->codec = avcodec_find_decoder((*ipt_ctx)->fmt_ctx->streams[0]->codecpar->codec_id);
    (*ipt_ctx)->stream = avformat_new_stream((*ipt_ctx)->fmt_ctx, (*ipt_ctx)->codec);
    (*ipt_ctx)->codec_ctx = avcodec_alloc_context3((*ipt_ctx)->codec);

    AVDictionary *codec_options = NULL;
    char str_framerate[8] = {0};
    char str_video_size[16] = {0};
    sprintf(str_framerate, "%d", (*ipt_ctx)->fps);
    sprintf(str_video_size, "%dx%d", width, height);
    av_dict_set(&codec_options, "video_size", str_video_size, 0);
    av_dict_set(&codec_options, "framerate", str_framerate, 0);
    av_dict_set(&codec_options, "preset", "superfast", 0);
    av_dict_set(&codec_options, "pixel_format", "yuyv422", 0);
    avcodec_parameters_to_context((*ipt_ctx)->codec_ctx, (*ipt_ctx)->fmt_ctx->streams[0]->codecpar);
    if (avcodec_open2((*ipt_ctx)->codec_ctx, (*ipt_ctx)->codec, &codec_options) != 0)
    {
        fprintf(stderr, "cannot initialize video decoder!\n");
        DeallocateHelpers((*ipt_ctx));
        (*ipt_ctx) = NULL;
        return RuntimeExceion;
    }
    return OkException;
}

void AVCameraDestroy(AVCameraHelpers *ipt_ctx)
{
    if (ipt_ctx)
    {
        if (ipt_ctx->fmt_ctx->pb)
        {
            avio_close(ipt_ctx->fmt_ctx->pb);
            fprintf(stderr, "avio_close.ipt_ctx\n");
        }
        if (ipt_ctx->fmt_ctx)
        {
            avformat_free_context(ipt_ctx->fmt_ctx);
            fprintf(stderr, "avformat_free_context.ipt_ctx\n");
        }

        DeallocateHelpers(ipt_ctx);
    }
}