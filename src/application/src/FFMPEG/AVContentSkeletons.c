#include <FFMPEG/AVContentSkeletons.h>

void AVContentInit()
{
#if LIBAVCODEC_VERSION_INT < AV_VERSION_INT(58, 9, 100)
    av_register_all();
#endif
    avdevice_register_all();
    avformat_network_init();
    av_log_set_level(AV_LOG_DEBUG);
}

void log_packet(const AVFormatContext *fmt_ctx, const AVPacket *pkt)
{
    AVRational *time_base = &fmt_ctx->streams[pkt->stream_index]->time_base;

    av_log(NULL, AV_LOG_DEBUG,
           "pts:%s pts_time:%s dts:%s dts_time:%s duration:%s duration_time:%s stream_index:%d\n",
           av_ts2str(pkt->pts), av_ts2timestr(pkt->pts, time_base),
           av_ts2str(pkt->dts), av_ts2timestr(pkt->dts, time_base),
           av_ts2str(pkt->duration), av_ts2timestr(pkt->duration, time_base),
           pkt->stream_index);
}

// exp_t AVImagestoYU420PInit(AVStreamSkeleton **__stream_skell, AVContentSkeleton *ctx_skel, AVFrame *inframe)
// {
//     AVStreamSkeleton *stream_skell = NULL;
//     if (!(stream_skell = (AVStreamSkeleton *)SkeletonAllocate((void **)__stream_skell, sizeof(AVStreamSkeleton))))
//         return exp_fail;
//     if (!inframe)
//         return exp_fail;
//     stream_skell->inframe = inframe;
//     stream_skell->width = ctx_skel->codec_ctx->width;
//     stream_skell->height = ctx_skel->codec_ctx->height;
//     stream_skell->pix_fmt = ctx_skel->codec_ctx->pix_fmt;

//     int nbytes = av_image_get_buffer_size(stream_skell->pix_fmt, stream_skell->width, stream_skell->height, 32);

//     uint8_t *video_outbuf = (uint8_t *)av_malloc(nbytes);

//     av_image_fill_arrays(stream_skell->inframe->data, stream_skell->inframe->linesize, video_outbuf, AV_PIX_FMT_YUV420P,
//                          stream_skell->width, stream_skell->height, 1);
//     stream_skell->inframe->width = stream_skell->width;
//     stream_skell->inframe->height = stream_skell->height;
//     stream_skell->inframe->format = stream_skell->pix_fmt;
//     // 像素格式转换YU420

//     stream_skell->img_convert_ctx = sws_getContext(stream_skell->width, stream_skell->height, stream_skell->pix_fmt,
//                                                    stream_skell->width, stream_skell->height, stream_skell->pix_fmt, SWS_BICUBIC, NULL, NULL, NULL);

//     if (!stream_skell->img_convert_ctx)
//     {
//         fprintf(stderr, "fail to sws_getCachedContext\n");
//         return exp_fail;
//     }
//     av_free(video_outbuf);
//     return exp_ok;
// }

// exp_t AVImagestoYU420PScaleing(AVStreamSkeleton *stream_skell, AVFrame *outframe)
// {
//     if (!stream_skell)
//         return exp_fail;
//     if (!stream_skell->img_convert_ctx)
//         return exp_fail;

//     // h264格式转换
//     sws_scale(stream_skell->img_convert_ctx,
//               (const uint8_t *const *)stream_skell->inframe->data,
//               stream_skell->inframe->linesize,
//               0,
//               stream_skell->height,
//               outframe->data,
//               outframe->linesize);
//     return exp_ok;
// }
// void AVImagestoYU420PDeallocate(AVStreamSkeleton *stream_skell)
// {
//     if (stream_skell)
//     {
//         if (stream_skell->img_convert_ctx)
//         {
//             sws_freeContext(stream_skell->img_convert_ctx);
//         }
//         SkeletonDeallocate(stream_skell);
//     }
// }