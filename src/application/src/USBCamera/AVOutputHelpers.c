#include <USBCamera/AVOutputHelpers.h>
#include <USBCamera/AVCameraHelpers.h>

#include <Helpers/Allocate.h>

exp_t AVOutputInit(
    AVOutputHelpers **opt_ctx,
    const char *output_path,
    const char *output_format,
    int width,
    int height,
    int fps)
{

    if (!AllocateHelpers((void **)opt_ctx, sizeof(AVOutputHelpers)))
    {
        fprintf(stderr, "LiveStreamInit opt_ctx\n");
        return exp_nullpointer;
    }
    (*opt_ctx)->width = width;
    (*opt_ctx)->height = height;
    (*opt_ctx)->fps = fps;

    if (avformat_alloc_output_context2(&((*opt_ctx)->fmt_ctx), NULL, output_format, NULL) != 0)
    {
        fprintf(stderr, "cannot initialize output format context!\n");
        DeallocateHelpers((*opt_ctx));
        return exp_runtime;
    }
    if (avio_open2(&((*opt_ctx)->fmt_ctx->pb), output_path, AVIO_FLAG_WRITE, NULL, NULL) != 0)
    {
        fprintf(stderr, "could not open IO context!\n");
        DeallocateHelpers((*opt_ctx));
        return exp_runtime;
    }
    (*opt_ctx)->codec = avcodec_find_encoder(AV_CODEC_ID_H264);
    (*opt_ctx)->stream = avformat_new_stream((*opt_ctx)->fmt_ctx, (*opt_ctx)->codec);
    (*opt_ctx)->codec_ctx = avcodec_alloc_context3((*opt_ctx)->codec);
    const AVRational dst_fps = {(*opt_ctx)->fps, 1};

    (*opt_ctx)->codec_ctx->codec_tag = 0;
    (*opt_ctx)->codec_ctx->codec_id = AV_CODEC_ID_H264;
    (*opt_ctx)->codec_ctx->codec_type = AVMEDIA_TYPE_VIDEO;
    (*opt_ctx)->codec_ctx->width = (*opt_ctx)->width;
    (*opt_ctx)->codec_ctx->height = (*opt_ctx)->height;
    (*opt_ctx)->codec_ctx->gop_size = 12;
    (*opt_ctx)->codec_ctx->pix_fmt = AV_PIX_FMT_YUV420P;
    (*opt_ctx)->codec_ctx->framerate = dst_fps;
    (*opt_ctx)->codec_ctx->time_base = av_inv_q(dst_fps);

    if ((*opt_ctx)->fmt_ctx->oformat->flags & AVFMT_GLOBALHEADER)
    {
        (*opt_ctx)->codec_ctx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    }
    if (avcodec_parameters_from_context((*opt_ctx)->stream->codecpar, (*opt_ctx)->codec_ctx) != 0)
    {
        fprintf(stderr, "could not initialize stream codec parameters!\n");
        DeallocateHelpers((*opt_ctx));
        return exp_runtime;
    }

    AVDictionary *codec_options = NULL;
    av_dict_set(&codec_options, "profile", "high", 0);
    av_dict_set(&codec_options, "preset", "superfast", 0);
    av_dict_set(&codec_options, "tune", "zerolatency", 0);

    // open video encoder
    if (avcodec_open2((*opt_ctx)->codec_ctx, (*opt_ctx)->codec, &codec_options) != 0)
    {
        fprintf(stderr, "could not open video encoder!\n");
        DeallocateHelpers((*opt_ctx));
        return exp_runtime;
    }
    (*opt_ctx)->stream->codecpar->extradata = (*opt_ctx)->codec_ctx->extradata;
    (*opt_ctx)->stream->codecpar->extradata_size = (*opt_ctx)->codec_ctx->extradata_size;

    av_dump_format((*opt_ctx)->fmt_ctx, 0, output_path, 1);

    return exp_ok;
}



void AVOutputDestroy(AVOutputHelpers *opt_ctx)
{

    avio_close(opt_ctx->fmt_ctx->pb);
    fprintf(stderr, "avio_close.opt_ctx\n");
    avformat_free_context(opt_ctx->fmt_ctx);
    fprintf(stderr, "avformat_free_context.opt_ctx\n");
    DeallocateHelpers(opt_ctx);
}




