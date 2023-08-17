#include <USBCamera/AVManagerHelpers.h>

exp_t AVManagerRuning(
    AVManagerHelpers *manage,
    AVCameraHelpers *ipt_ctx,
    AVOutputHelpers *opt_ctx)
{

    if ((manage == NULL) && (ipt_ctx == NULL) && (opt_ctx == NULL))
    {
        fprintf(stderr, "UpStreamingStart  IllegalArgumentException\n");
        return exp_illegalargument;
    }

    manage->loop = true;

    if (avformat_write_header(opt_ctx->fmt_ctx, NULL) != 0)
    {
        fprintf(stderr, "could not write header to ouput context!\n");
        return exp_runtime;
    }
    opt_ctx->frame = NULL;
    opt_ctx->outframe = av_frame_alloc();
    opt_ctx->pkt = av_packet_alloc();

    int nbytes = av_image_get_buffer_size(opt_ctx->codec_ctx->pix_fmt, opt_ctx->codec_ctx->width, opt_ctx->codec_ctx->height, 32);
    uint8_t *video_outbuf = (uint8_t *)av_malloc(nbytes);
    av_image_fill_arrays(opt_ctx->outframe->data, opt_ctx->outframe->linesize, video_outbuf, AV_PIX_FMT_YUV420P, opt_ctx->codec_ctx->width, opt_ctx->codec_ctx->height, 1);
    opt_ctx->outframe->width = opt_ctx->codec_ctx->width;
    opt_ctx->outframe->height = opt_ctx->codec_ctx->height;
    opt_ctx->outframe->format = opt_ctx->codec_ctx->pix_fmt;
    // 像素格式转换YU420
    struct SwsContext *img_convert_ctx = NULL;
    img_convert_ctx = sws_getContext(ipt_ctx->codec_ctx->width, ipt_ctx->codec_ctx->height, ipt_ctx->codec_ctx->pix_fmt,
                                     opt_ctx->codec_ctx->width, opt_ctx->codec_ctx->height, opt_ctx->codec_ctx->pix_fmt, SWS_BICUBIC, NULL, NULL, NULL);

    if (!img_convert_ctx)
    {
        fprintf(stderr, "fail to sws_getCachedContext\n");
        return exp_runtime;
    }
    av_new_packet(opt_ctx->pkt, 0);

    long pts = 0;
    opt_ctx->frame = av_frame_alloc();
    while (manage->loop)
    {
        av_init_packet(opt_ctx->pkt);
        if (av_read_frame(ipt_ctx->fmt_ctx, opt_ctx->pkt) >= 0)
        {
            if ((avcodec_send_packet(ipt_ctx->codec_ctx, opt_ctx->pkt) >= 0) && (avcodec_receive_frame(ipt_ctx->codec_ctx, opt_ctx->frame) >= 0))
            {
                av_packet_unref(opt_ctx->pkt);
                av_init_packet(opt_ctx->pkt);
                // h264格式转换
                sws_scale(img_convert_ctx, (const uint8_t *const *)opt_ctx->frame->data, opt_ctx->frame->linesize, 0, ipt_ctx->codec_ctx->height, opt_ctx->outframe->data, opt_ctx->outframe->linesize);
                // 每一帧加1
                opt_ctx->outframe->pts = pts++;
                if ((avcodec_send_frame(opt_ctx->codec_ctx, opt_ctx->outframe) >= 0) && (avcodec_receive_packet(opt_ctx->codec_ctx, opt_ctx->pkt) >= 0))
                {
                    opt_ctx->pkt->pts = av_rescale_q(opt_ctx->pkt->pts, opt_ctx->codec_ctx->time_base, opt_ctx->stream->time_base);
                    opt_ctx->pkt->dts = av_rescale_q(opt_ctx->pkt->dts, opt_ctx->codec_ctx->time_base, opt_ctx->stream->time_base);
                   // log_packet(opt_ctx->fmt_ctx, opt_ctx->pkt);
                    av_interleaved_write_frame(opt_ctx->fmt_ctx, opt_ctx->pkt);
                }
            }
        }
        av_packet_unref(opt_ctx->pkt);
    }

    av_write_trailer(opt_ctx->fmt_ctx);
    av_packet_unref(opt_ctx->pkt);
    av_frame_free(&opt_ctx->outframe);
    av_frame_free(&opt_ctx->frame);
    av_free(video_outbuf);
    fprintf(stderr, "done.\n");
    return exp_ok;
}

AVManagerHelpers *AVManagerInit(AVManagerHelpers **manager)
{
#if LIBAVCODEC_VERSION_INT < AV_VERSION_INT(58, 9, 100)
    av_register_all();
#endif
    avdevice_register_all();
    avformat_network_init();
    av_log_set_level(AV_LOG_DEBUG);
    if (!AllocateHelpers((void **)manager, sizeof(AVManagerHelpers)))
    {
        fprintf(stderr, "AVManagerInit *factory\n");
        return NULL;
    }

    (*manager)->loop = false;

    return (*manager);
}

void AVManagerDestroy(AVManagerHelpers *manager)
{
    DeallocateHelpers(manager);
}

static void log_packet(const AVFormatContext *fmt_ctx, const AVPacket *pkt)
{
    AVRational *time_base = &fmt_ctx->streams[pkt->stream_index]->time_base;

    av_log(NULL, AV_LOG_DEBUG,
           "pts:%s pts_time:%s dts:%s dts_time:%s duration:%s duration_time:%s stream_index:%d\n",
           av_ts2str(pkt->pts), av_ts2timestr(pkt->pts, time_base),
           av_ts2str(pkt->dts), av_ts2timestr(pkt->dts, time_base),
           av_ts2str(pkt->duration), av_ts2timestr(pkt->duration, time_base),
           pkt->stream_index);
}