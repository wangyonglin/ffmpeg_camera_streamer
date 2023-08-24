#include <FFMPEG/AVFilterSkeletons.h>

void AVFilterClear(AVFilterSkeleton *filter_skell)
{
    if (filter_skell)
    {
        if (filter_skell->filter_graph)
        {
            avfilter_graph_free(&filter_skell->filter_graph);
        }
        SkeletonDeallocate(filter_skell);
    }
}

int AVFilterInit(AVFilterSkeleton **__filter_skell, AVInputContextSkeleton *ipt_ctx_skell,
                 AVOutputContextSkeleton *opt_ctx_skell, const char *filter_spec)
{
    int ret = 0;
    AVFilterSkeleton *filter_skell = NULL;
    if (!(filter_skell = (AVFilterSkeleton *)SkeletonAllocate((void **)__filter_skell, sizeof(AVFilterSkeleton))))
    {
        av_log(NULL, AV_LOG_ERROR, "Cannot allocate a AVFilterInit filter_skell fail\n");
        return -1;
    }
    filter_skell->ipt_ctx_skell = ipt_ctx_skell;
    filter_skell->opt_ctx_skell = opt_ctx_skell;
    int ipt_stream_index = -1;

    /* select the video stream */
    ipt_stream_index = av_find_best_stream(ipt_ctx_skell->fmt_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, &(ipt_ctx_skell->codec), 0);
    if (ipt_stream_index < 0)
    {
        av_log(NULL, AV_LOG_ERROR, "Cannot find a video stream in the input file\n");

        return -1;
    }

    char args[512];

    const AVFilter *buffersrc = NULL;
    const AVFilter *buffersink = NULL;
    // AVFilterContext *buffersrc_ctx = NULL;
    // AVFilterContext *buffersink_ctx = NULL;
    AVFilterInOut *outputs = avfilter_inout_alloc();
    AVFilterInOut *inputs = avfilter_inout_alloc();
    filter_skell->filter_graph = avfilter_graph_alloc();
    AVBufferSinkParams *buffersink_params;
    if (!outputs || !inputs || !filter_skell->filter_graph)
    {
        ret = AVERROR(ENOMEM);
        goto end;
    }
    AVRational time_base = ipt_ctx_skell->fmt_ctx->streams[ipt_stream_index]->time_base;
    enum AVPixelFormat pix_fmts[] = {AV_PIX_FMT_YUYV422, AV_PIX_FMT_NONE};
    if (ipt_ctx_skell->codec_ctx->codec_type == AVMEDIA_TYPE_VIDEO)
    {
        buffersrc = avfilter_get_by_name("buffer");
        buffersink = avfilter_get_by_name("buffersink");
        if (!buffersrc || !buffersink)
        {
            av_log(NULL, AV_LOG_ERROR, "filtering source or sink element not found\n");
            ret = AVERROR_UNKNOWN;
            goto end;
        }

        snprintf(args, sizeof(args),
                 "video_size=%dx%d:pix_fmt=%d:time_base=%d/%d:pixel_aspect=%d/%d",
                 ipt_ctx_skell->codec_ctx->width, ipt_ctx_skell->codec_ctx->height, ipt_ctx_skell->codec_ctx->pix_fmt,
                 time_base.num, time_base.den,
                 ipt_ctx_skell->codec_ctx->sample_aspect_ratio.num,
                 ipt_ctx_skell->codec_ctx->sample_aspect_ratio.den);

        ret = avfilter_graph_create_filter(&(filter_skell->buffersrc_ctx), buffersrc, "in",
                                           args, NULL, filter_skell->filter_graph);
        if (ret < 0)
        {
            av_log(NULL, AV_LOG_ERROR, "Cannot create buffer source\n");
            goto end;
        }
        /* buffer video sink: to terminate the filter chain. */
        buffersink_params = av_buffersink_params_alloc();
        buffersink_params->pixel_fmts = pix_fmts;

        ret = avfilter_graph_create_filter(&(filter_skell->buffersink_ctx), buffersink, "out",
                                           NULL, buffersink_params, filter_skell->filter_graph);
        av_free(buffersink_params);
        if (ret < 0)
        {
            av_log(NULL, AV_LOG_ERROR, "Cannot create buffer sink\n");
            goto end;
        }

        ret = av_opt_set_bin(filter_skell->buffersink_ctx, "pix_fmts",
                             (uint8_t *)&opt_ctx_skell->codec_ctx->pix_fmt, sizeof(opt_ctx_skell->codec_ctx->pix_fmt),
                             AV_OPT_SEARCH_CHILDREN);
        if (ret < 0)
        {
            av_log(NULL, AV_LOG_ERROR, "Cannot set output pixel format\n");
            goto end;
        }
    
    }
    else if (ipt_ctx_skell->codec_ctx->codec_type == AVMEDIA_TYPE_AUDIO)
    {
        buffersrc = avfilter_get_by_name("abuffer");
        buffersink = avfilter_get_by_name("abuffersink");
        if (!buffersrc || !buffersink)
        {
            av_log(NULL, AV_LOG_ERROR, "filtering source or sink element not found\n");
            ret = AVERROR_UNKNOWN;
            goto end;
        }

        if (!ipt_ctx_skell->codec_ctx->channel_layout)
            ipt_ctx_skell->codec_ctx->channel_layout =
                av_get_default_channel_layout(ipt_ctx_skell->codec_ctx->channels);
        snprintf(args, sizeof(args),
                 "time_base=%d/%d:sample_rate=%d:sample_fmt=%s:channel_layout=0x%" PRIx64,
                 ipt_ctx_skell->codec_ctx->time_base.num, ipt_ctx_skell->codec_ctx->time_base.den, ipt_ctx_skell->codec_ctx->sample_rate,
                 av_get_sample_fmt_name(ipt_ctx_skell->codec_ctx->sample_fmt),
                 ipt_ctx_skell->codec_ctx->channel_layout);
        ret = avfilter_graph_create_filter(&filter_skell->buffersrc_ctx, buffersrc, "in",
                                           args, NULL, filter_skell->filter_graph);
        if (ret < 0)
        {
            av_log(NULL, AV_LOG_ERROR, "Cannot create audio buffer source\n");
            goto end;
        }

        ret = avfilter_graph_create_filter(&filter_skell->buffersink_ctx, buffersink, "out",
                                           NULL, NULL, filter_skell->filter_graph);
        if (ret < 0)
        {
            av_log(NULL, AV_LOG_ERROR, "Cannot create audio buffer sink\n");
            goto end;
        }

        ret = av_opt_set_bin(filter_skell->buffersink_ctx, "sample_fmts",
                             (uint8_t *)&opt_ctx_skell->codec_ctx->sample_fmt, sizeof(opt_ctx_skell->codec_ctx->sample_fmt),
                             AV_OPT_SEARCH_CHILDREN);
        if (ret < 0)
        {
            av_log(NULL, AV_LOG_ERROR, "Cannot set output sample format\n");
            goto end;
        }

        ret = av_opt_set_bin(filter_skell->buffersink_ctx, "channel_layouts",
                             (uint8_t *)&opt_ctx_skell->codec_ctx->channel_layout,
                             sizeof(opt_ctx_skell->codec_ctx->channel_layout), AV_OPT_SEARCH_CHILDREN);
        if (ret < 0)
        {
            av_log(NULL, AV_LOG_ERROR, "Cannot set output channel layout\n");
            goto end;
        }

        ret = av_opt_set_bin(filter_skell->buffersink_ctx, "sample_rates",
                             (uint8_t *)&opt_ctx_skell->codec_ctx->sample_rate, sizeof(opt_ctx_skell->codec_ctx->sample_rate),
                             AV_OPT_SEARCH_CHILDREN);
        if (ret < 0)
        {
            av_log(NULL, AV_LOG_ERROR, "Cannot set output sample rate\n");
            goto end;
        }
    }
    else
    {
        ret = AVERROR_UNKNOWN;
        goto end;
    }

    /* Endpoints for the filter graph. */
    outputs->name = av_strdup("in");
    outputs->filter_ctx = filter_skell->buffersrc_ctx;
    outputs->pad_idx = 0;
    outputs->next = NULL;

    inputs->name = av_strdup("out");
    inputs->filter_ctx = filter_skell->buffersink_ctx;
    inputs->pad_idx = 0;
    inputs->next = NULL;

    if (!outputs->name || !inputs->name)
    {
        ret = AVERROR(ENOMEM);
        goto end;
    }

    if ((ret = avfilter_graph_parse_ptr(filter_skell->filter_graph, filter_spec,
                                        &inputs, &outputs, NULL)) < 0)
        goto end;

    if ((ret = avfilter_graph_config(filter_skell->filter_graph, NULL)) < 0)
        goto end;

end:
    avfilter_inout_free(&inputs);
    avfilter_inout_free(&outputs);

    return ret;
}