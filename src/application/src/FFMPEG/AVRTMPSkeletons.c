#include <FFMPEG/AVRTMPSkeletons.h>

void AVRTMPClear(AVOutputContextSkeleton *opt_ctx_skell)
{
    if (opt_ctx_skell)
    {

        if (opt_ctx_skell->fmt_ctx)
        {
            if (opt_ctx_skell->fmt_ctx->pb)
            {
                avio_close(opt_ctx_skell->fmt_ctx->pb);
                fprintf(stderr, "avio_close.opt_ctx_skell->fmt_ctx\n");
            }
            avformat_free_context(opt_ctx_skell->fmt_ctx);
            fprintf(stderr, "avformat_free_context.opt_ctx_skell->fmt_ctx\n");
        }
        SkeletonDeallocate(opt_ctx_skell);
    }
}

exp_t AVRTMPInit(AVOutputContextSkeleton **__opt_ctx_skell, const char *output_path,
                 int width,
                 int height,
                 int fps)
{
    AVContextSkeleton *opt_ctx_skell = NULL;

    if (!(opt_ctx_skell = (AVContextSkeleton *)SkeletonAllocate((void **)__opt_ctx_skell, sizeof(AVContextSkeleton))))
    {
        return exp_fail;
    }

    if (avformat_alloc_output_context2(&(opt_ctx_skell->fmt_ctx), NULL, "flv", NULL) != 0)
    {
        fprintf(stderr, "cannot initialize output format context!\n");

        return exp_fail;
    }
    if (avio_open2(&(opt_ctx_skell->fmt_ctx->pb), output_path, AVIO_FLAG_WRITE, NULL, NULL) != 0)
    {
        fprintf(stderr, "could not open IO context!\n");

        return exp_fail;
    }
    opt_ctx_skell->codec = avcodec_find_encoder(AV_CODEC_ID_H264);
    opt_ctx_skell->stream = avformat_new_stream(opt_ctx_skell->fmt_ctx, opt_ctx_skell->codec);
    opt_ctx_skell->codec_ctx = avcodec_alloc_context3(opt_ctx_skell->codec);
    const AVRational dst_fps = {fps, 1};

    opt_ctx_skell->codec_ctx->codec_tag = 0;
    opt_ctx_skell->codec_ctx->codec_id = AV_CODEC_ID_H264;
    opt_ctx_skell->codec_ctx->codec_type = AVMEDIA_TYPE_VIDEO;
    opt_ctx_skell->codec_ctx->width = width;
    opt_ctx_skell->codec_ctx->height = height;
    opt_ctx_skell->codec_ctx->gop_size = 12;
    opt_ctx_skell->codec_ctx->pix_fmt = AV_PIX_FMT_YUV420P;
    opt_ctx_skell->codec_ctx->framerate = dst_fps;
    opt_ctx_skell->codec_ctx->time_base = av_inv_q(dst_fps);

    if (opt_ctx_skell->fmt_ctx->oformat->flags & AVFMT_GLOBALHEADER)
    {
        opt_ctx_skell->codec_ctx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    }
    if (avcodec_parameters_from_context(opt_ctx_skell->stream->codecpar, opt_ctx_skell->codec_ctx) != 0)
    {
        fprintf(stderr, "could not initialize stream codec parameters!\n");
        return exp_fail;
    }

    AVDictionary *codec_options = NULL;
    av_dict_set(&codec_options, "profile", "high", 0);
    av_dict_set(&codec_options, "preset", "superfast", 0);
    av_dict_set(&codec_options, "tune", "zerolatency", 0);

    // open video encoder
    if (avcodec_open2(opt_ctx_skell->codec_ctx, opt_ctx_skell->codec, &codec_options) != 0)
    {
        fprintf(stderr, "could not open video encoder!\n");

        return exp_fail;
    }
    opt_ctx_skell->stream->codecpar->extradata = opt_ctx_skell->codec_ctx->extradata;
    opt_ctx_skell->stream->codecpar->extradata_size = opt_ctx_skell->codec_ctx->extradata_size;

    av_dump_format(opt_ctx_skell->fmt_ctx, 0, output_path, 1);

    return exp_ok;
}

exp_t AVRTMPWriteFrame(AVOutputContextSkeleton *opt_ctx_skell, AVInputContextSkeleton *ipt_ctx_skell, bl_t *loop)
{
    if ((opt_ctx_skell == NULL) && (ipt_ctx_skell == NULL))
    {
        return exp_fail;
    }

    if (avformat_write_header(opt_ctx_skell->fmt_ctx, NULL) != 0)
    {
        fprintf(stderr, "could not write header to ouput context!\n");
        return exp_fail;
    }

    AVFrame *opt_frame = av_frame_alloc();

    int nbytes = av_image_get_buffer_size(opt_ctx_skell->codec_ctx->pix_fmt, opt_ctx_skell->codec_ctx->width, opt_ctx_skell->codec_ctx->height, 32);
    uint8_t *video_outbuf = (uint8_t *)av_malloc(nbytes);
    av_image_fill_arrays(opt_frame->data, opt_frame->linesize, video_outbuf, AV_PIX_FMT_YUV420P, opt_ctx_skell->codec_ctx->width, opt_ctx_skell->codec_ctx->height, 1);
    opt_frame->width = opt_ctx_skell->codec_ctx->width;
    opt_frame->height = opt_ctx_skell->codec_ctx->height;
    opt_frame->format = opt_ctx_skell->codec_ctx->pix_fmt;
    // 像素格式转换YU420
    struct SwsContext *img_convert_ctx = NULL;
    img_convert_ctx = sws_getContext(ipt_ctx_skell->codec_ctx->width, ipt_ctx_skell->codec_ctx->height, ipt_ctx_skell->codec_ctx->pix_fmt,
                                     opt_ctx_skell->codec_ctx->width, opt_ctx_skell->codec_ctx->height, opt_ctx_skell->codec_ctx->pix_fmt, SWS_BICUBIC, NULL, NULL, NULL);

    if (!img_convert_ctx)
    {
        fprintf(stderr, "fail to sws_getCachedContext\n");
        return exp_fail;
    }

    AVPacket pkt = {.data = NULL, .size = 0};
    AVFrame *ipt_frame = av_frame_alloc();

    // av_new_packet(&pkt, 0);
    long pts = 0;
    while (*loop)
    {
        av_init_packet(&pkt);
        if (av_read_frame(ipt_ctx_skell->fmt_ctx, &pkt) >= 0)
        {
            if ((avcodec_send_packet(ipt_ctx_skell->codec_ctx, &pkt) >= 0) && (avcodec_receive_frame(ipt_ctx_skell->codec_ctx, ipt_frame) >= 0))
            {
                av_packet_unref(&pkt);
                av_init_packet(&pkt);
                // h264格式转换
                sws_scale(img_convert_ctx,
                          (const uint8_t *const *)ipt_frame->data,
                          ipt_frame->linesize,
                          0,
                          ipt_ctx_skell->codec_ctx->height,
                          opt_frame->data,
                          opt_frame->linesize);
                // 每一帧加1
                opt_frame->pts = pts++;
                if ((avcodec_send_frame(opt_ctx_skell->codec_ctx, opt_frame) >= 0) && (avcodec_receive_packet(opt_ctx_skell->codec_ctx, &pkt) >= 0))
                {
                    pkt.pts = av_rescale_q(pkt.pts, opt_ctx_skell->codec_ctx->time_base, opt_ctx_skell->stream->time_base);
                    pkt.dts = av_rescale_q(pkt.dts, opt_ctx_skell->codec_ctx->time_base, opt_ctx_skell->stream->time_base);

                    // AVContentLogger(opt_skel->fmt_ctx, &pkt);
                    av_interleaved_write_frame(opt_ctx_skell->fmt_ctx, &pkt);
                }
            }
        }
        av_packet_unref(&pkt);
    }
    if (av_write_trailer(opt_ctx_skell->fmt_ctx) != 0)
    {
        fprintf(stderr, "could not write trailer to ouput context!\n");
        return exp_fail;
    }

    av_frame_free(&opt_frame);
    av_frame_free(&ipt_frame);
    av_free(video_outbuf);
    av_packet_unref(&pkt);
    fprintf(stderr, "done.\n");
    return exp_ok;
}

exp_t AVRTMPFilterWriteFrame(AVOutputContextSkeleton *opt_ctx_skell, AVFilterSkeleton *filter_skell, bl_t *loop)
{
    if ((!opt_ctx_skell) && (!filter_skell))
    {
        fprintf(stderr, "AVRTMPFilterWriteFrame null!\n");
        return exp_fail;
    }
    AVInputContextSkeleton *ipt_ctx_skell = filter_skell->ipt_ctx_skell;
    if (!ipt_ctx_skell)
    {
        fprintf(stderr, "AVRTMPFilterWriteFrame  ipt_ctx_skell null!\n");
        return exp_fail;
    }
    if (avformat_write_header(opt_ctx_skell->fmt_ctx, NULL) != 0)
    {
        fprintf(stderr, "could not write header to ouput context!\n");
        return exp_fail;
    }
    unsigned char *frame_buffer_in;
    unsigned char *frame_buffer_out;
    AVPacket pkt = {.data = NULL, .size = 0};
    AVFrame *frame_in = av_frame_alloc();
    frame_buffer_in = (unsigned char *)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_YUV420P, ipt_ctx_skell->codec_ctx->width, ipt_ctx_skell->codec_ctx->height, 1));
    av_image_fill_arrays(frame_in->data, frame_in->linesize, frame_buffer_in,
                         AV_PIX_FMT_YUV420P, ipt_ctx_skell->codec_ctx->width, ipt_ctx_skell->codec_ctx->height, 1);

    AVFrame *frame_out = av_frame_alloc();
    frame_out = av_frame_alloc();
    frame_buffer_out = (unsigned char *)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_YUV420P, ipt_ctx_skell->codec_ctx->width, ipt_ctx_skell->codec_ctx->height, 1));
    av_image_fill_arrays(frame_out->data, frame_out->linesize, frame_buffer_out,
                         AV_PIX_FMT_YUV420P, ipt_ctx_skell->codec_ctx->width, ipt_ctx_skell->codec_ctx->height, 1);

    // av_new_packet(&pkt, 0);
    long pts = 0;
    int ret = -1;
    while (*loop)
    {
        av_init_packet(&pkt);
        if (av_read_frame(ipt_ctx_skell->fmt_ctx, &pkt) >= 0)
        {
            if ((avcodec_send_packet(ipt_ctx_skell->codec_ctx, &pkt) >= 0) && (avcodec_receive_frame(ipt_ctx_skell->codec_ctx, frame_in) >= 0))
            {
                av_packet_unref(&pkt);
                av_init_packet(&pkt);

                /* push the decoded frame into the filtergraph */

                if ((av_buffersrc_add_frame_flags(filter_skell->buffersrc_ctx, frame_in, AV_BUFFERSRC_FLAG_KEEP_REF) >= 0) && (av_buffersink_get_frame(filter_skell->buffersink_ctx, frame_out) >= 0))
                {
                    /* pull filtered frames from the filtergraph */
                    frame_out->pts = pts++;
                    if ((avcodec_send_frame(opt_ctx_skell->codec_ctx, frame_out) >= 0) && (avcodec_receive_packet(opt_ctx_skell->codec_ctx, &pkt) >= 0))
                    {
                        pkt.pts = av_rescale_q(pkt.pts, opt_ctx_skell->codec_ctx->time_base, opt_ctx_skell->stream->time_base);
                        pkt.dts = av_rescale_q(pkt.dts, opt_ctx_skell->codec_ctx->time_base, opt_ctx_skell->stream->time_base);
                        av_interleaved_write_frame(opt_ctx_skell->fmt_ctx, &pkt);
                    }

                    av_frame_unref(frame_out);

                    av_frame_unref(frame_in);
                }
            }
        }
        av_packet_unref(&pkt);
    }
    if (av_write_trailer(opt_ctx_skell->fmt_ctx) != 0)
    {
        fprintf(stderr, "could not write trailer to ouput context!\n");
        return exp_fail;
    }
    av_frame_free(&frame_in);
    av_frame_free(&frame_out);
    av_free(frame_buffer_in);
    av_free(frame_buffer_out);
    av_packet_unref(&pkt);
    fprintf(stderr, "done.\n");
    return exp_ok;
}
