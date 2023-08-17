#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libavformat/avformat.h>
#include <libavdevice/avdevice.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
const char *usbcamera = "/dev/video0";
const char *short_name = "v4l2";

#define OUTPUT_URL "rtmp://192.168.1.2:1935/live/stream"
int main()
{
	avdevice_register_all(); // 注册设备

	AVInputFormat *videoInputFormat = av_find_input_format(short_name);
	AVFormatContext *videoInputContext = NULL;
	if (avformat_open_input(&videoInputContext, usbcamera, videoInputFormat, NULL) != 0)
	{
		fprintf(stderr, "无法打开视频设备\n");
		return -1;
	}
	AVDictionary *options = NULL;
	av_dict_set(&options, "video_size", "800x600", 0);
	av_dict_set(&options, "framerate", "24", 0);
	// av_dict_set(&options, "pixel_format", "uyvy422", 0);
	av_dict_set(&options, "probesize", "7000000", 0);
	if (avformat_find_stream_info(videoInputContext, &options) < 0)
	{
		fprintf(stderr, "无法获取视频流信息\n");
		return -1;
	}

	int videoStreamIndex = -1;
	for (int i = 0; i < videoInputContext->nb_streams; i++)
	{
		if (videoInputContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			videoStreamIndex = i;
			break;
		}
	}

	if (videoStreamIndex == -1)
	{
		fprintf(stderr, "找不到视频流\n");
		return -1;
	}

	AVCodecParameters *videoCodecParameters = videoInputContext->streams[videoStreamIndex]->codecpar;
	AVCodec *videoCodec = avcodec_find_decoder(videoCodecParameters->codec_id);
	if (videoCodec == NULL)
	{
		fprintf(stderr, "找不到视频解码器\n");
		return -1;
	}

	AVCodecContext *videoCodecContext = avcodec_alloc_context3(videoCodec);
	if (avcodec_parameters_to_context(videoCodecContext, videoCodecParameters) < 0)
	{
		fprintf(stderr, "无法初始化视频解码器上下文\n");
		return -1;
	}

	if (avcodec_open2(videoCodecContext, videoCodec, NULL) < 0)
	{
		fprintf(stderr, "无法打开视频解码器\n");
		return -1;
	}

	AVInputFormat *audioInputFormat = av_find_input_format("alsa");
	AVFormatContext *audioInputContext = NULL;
	if (avformat_open_input(&audioInputContext, "default", audioInputFormat, NULL) != 0)
	{
		fprintf(stderr, "无法打开音频设备\n");
		return -1;
	}

	if (avformat_find_stream_info(audioInputContext, NULL) < 0)
	{
		fprintf(stderr, "无法获取音频流信息\n");
		return -1;
	}

	int audioStreamIndex = -1;
	for (int i = 0; i < audioInputContext->nb_streams; i++)
	{
		if (audioInputContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
		{
			audioStreamIndex = i;
			break;
		}
	}

	if (audioStreamIndex == -1)
	{
		fprintf(stderr, "找不到音频流\n");
		return -1;
	}

	AVCodecParameters *audioCodecParameters = audioInputContext->streams[audioStreamIndex]->codecpar;
	AVCodec *audioCodec = avcodec_find_decoder(audioCodecParameters->codec_id);
	if (audioCodec == NULL)
	{
		fprintf(stderr, "找不到音频解码器\n");
		return -1;
	}

	AVCodecContext *audioCodecContext = avcodec_alloc_context3(audioCodec);
	if (avcodec_parameters_to_context(audioCodecContext, audioCodecParameters) < 0)
	{
		fprintf(stderr, "无法初始化音频解码器上下文\n");
		return -1;
	}

	if (avcodec_open2(audioCodecContext, audioCodec, NULL) < 0)
	{
		fprintf(stderr, "无法打开音频解码器\n");
		return -1;
	}

	AVFormatContext *outputContext = NULL;
	if (avformat_alloc_output_context2(&outputContext, NULL, "flv", OUTPUT_URL) < 0)
	{
		fprintf(stderr, "无法创建输出上下文\n");
		return -1;
	}

	AVStream *videoOutputStream = avformat_new_stream(outputContext, videoCodec);
	if (videoOutputStream == NULL)
	{
		fprintf(stderr, "无法创建视频输出流\n");
		return -1;
	}

	AVStream *audioOutputStream = avformat_new_stream(outputContext, audioCodec);
	if (audioOutputStream == NULL)
	{
		fprintf(stderr, "无法创建音频输出流\n");
		return -1;
	}

	if (avcodec_parameters_copy(videoOutputStream->codecpar, videoCodecParameters) < 0)
	{
		fprintf(stderr, "无法复制视频参数\n");
		return -1;
	}

	if (avcodec_parameters_copy(audioOutputStream->codecpar, audioCodecParameters) < 0)
	{
		fprintf(stderr, "无法复制音频参数\n");
		return -1;
	}

	if (avio_open(&outputContext->pb, OUTPUT_URL, AVIO_FLAG_WRITE) < 0)
	{
		fprintf(stderr, "无法打开输出URL\n");
		return -1;
	}
	av_dump_format(outputContext, 0, OUTPUT_URL, 1);
	if (avformat_write_header(outputContext, NULL) < 0)
	{
		fprintf(stderr, "无法写入输出文件头\n");
		return -1;
	}

	AVPacket packet;
	av_init_packet(&packet);

	AVFrame *videoFrame = av_frame_alloc();
	AVFrame *audioFrame = av_frame_alloc();

	while (1)
	{
		if (av_read_frame(videoInputContext, &packet) >= 0)
		{
			if (packet.stream_index == videoStreamIndex)
			{
				if (avcodec_send_packet(videoCodecContext, &packet) >= 0 && avcodec_receive_frame(videoCodecContext, videoFrame) >= 0)
				{
					// 处理视频帧
					// ...

					// 编码并写入输出流
					av_packet_unref(&packet);
					av_init_packet(&packet);
					if (avcodec_send_frame(videoCodecContext, videoFrame) >= 0 && avcodec_receive_packet(videoCodecContext, &packet) >= 0)
					{
						av_interleaved_write_frame(outputContext, &packet);
					}
				}
			}
			av_packet_unref(&packet);
		}

		if (av_read_frame(audioInputContext, &packet) >= 0)
		{
			if (packet.stream_index == audioStreamIndex)
			{
				if (avcodec_send_packet(audioCodecContext, &packet) >= 0 && avcodec_receive_frame(audioCodecContext, audioFrame) >= 0)
				{
					// 处理音频帧
					// ...

					// 编码并写入输出流
					av_packet_unref(&packet);
					av_init_packet(&packet);
					if (avcodec_send_frame(audioCodecContext, audioFrame) >= 0 && avcodec_receive_packet(audioCodecContext, &packet) >= 0)
					{
						av_interleaved_write_frame(outputContext, &packet);
					}
				}
			}
			av_packet_unref(&packet);
		}
	}

	av_write_trailer(outputContext);

	av_frame_free(&videoFrame);
	av_frame_free(&audioFrame);

	avformat_close_input(&videoInputContext);
	avformat_close_input(&audioInputContext);

	avcodec_free_context(&videoCodecContext);
	avcodec_free_context(&audioCodecContext);

	avio_close(outputContext->pb);
	avformat_free_context(outputContext);

	return 0;
}
