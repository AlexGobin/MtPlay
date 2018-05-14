#include "Mt_OpenVideo.h"

// pts 时间换算  转换成毫秒
static double r2d(AVRational r)
{
	return r.num == 0 || r.den == 0 ? 0. : (double)r.num / (double)r.den;
}

Mt_OpenVideo::Mt_OpenVideo()
{
	static bool isFrist = true;
	if (isFrist)
	{
		av_register_all();         //初始化
		avformat_network_init();		
		avcodec_register_all();//注册解码器
		isFrist = false;
	}
}


Mt_OpenVideo::~Mt_OpenVideo()
{
	close();
}

//打开视频文件
int Mt_OpenVideo::open(const char * filename)
{
	close();
	////分配空间
	//pFormatCtx = avformat_alloc_context();
	//if (pFormatCtx == NULL)
	//{
	//	std::cout << "avformat_alloc_context() failed!" << std::endl;
	//	return -1;
	//}

	AVDictionary * opts = NULL;
	//设置rtsp流以tcp协议打开
	av_dict_set(&opts, "rtsp_transport", "tcp", 0);
	//网络延时时间 rtsp 设置非常重要
	av_dict_set(&opts, "max_delay", "500", 0);

	//打开视频文件
	int ret = avformat_open_input(&pFormatCtx, filename, NULL, &opts);
	if (ret != 0)
	{
		std::cout << "avformat_open_input() failed!" << std::endl;
		return -1;
	}

	//再次获取流信息 
	ret = avformat_find_stream_info(pFormatCtx, NULL);
	if (ret < 0)
	{
		std::cout << "avformat_find_stream_info() failed" << std::endl;
		return -2;
	}

	//打印视频文件信息
	av_dump_format(pFormatCtx, 0, filename, 0);
	//获取视频总时长
	this->totalMs = (pFormatCtx->duration / AV_TIME_BASE) * 1000;
	std::cout << "视频总时长" << totalMs << std::endl;
	

	//获取视频
	videoStream = av_find_best_stream(pFormatCtx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
	avs = pFormatCtx->streams[videoStream];
	this->width = avs->codecpar->width;    //宽
	this->height = avs->codecpar->height;	//高
	this->fps = r2d(avs->avg_frame_rate);  //fps
	this->videoFMt = (int)pFormatCtx->streams[videoStream]->codec->pix_fmt;

	//获取音频
	audioStream = av_find_best_stream(pFormatCtx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
	//avs = pFormatCtx->streams[audioStream];
	this->channles = avs->codecpar->channels;    //通道数
	this->sampleRate = avs->codecpar->sample_rate;  //样本率
	return 0;
}


int Mt_OpenVideo::read(AVPacket **pkt)
{
	if (pFormatCtx == NULL)
	{
		std::cout << "The file is not open!" << std::endl;
		return -1;
	}
	AVPacket *packet = av_packet_alloc();
	int ret = av_read_frame(pFormatCtx, packet);
	if (ret != 0)
	{
		av_packet_free(&packet);
		std::cout << "read video failed!" << std::endl;
		return -2;
	}
	
	//计算pts dts
	packet->pts = packet->pts*(1000 * (r2d(pFormatCtx->streams[packet->stream_index]->time_base)));
	packet->dts = packet->dts*(1000 * (r2d(pFormatCtx->streams[packet->stream_index]->time_base)));

	//std::cout << "pts:" << packet->pts << "-----" << "dts:" << packet->dts << std::endl;	
	*pkt = packet;
	return 0;
}

//音视频分离器
bool Mt_OpenVideo::AVSeparate(AVPacket *pkt)
{
	if (pkt == NULL)
		return false;

	if (pkt->stream_index == videoStream)
	{
		//av_packet_free(&pkt);
		VideoQueue.push(pkt);
		return true;
	}

	else if (pkt->stream_index == audioStream)
	{
		//av_packet_free(&pkt);
		AudioQueue.push(pkt);
		return true;
	}
	av_packet_free(&pkt);
	return false;
}

//获取视频或音频信息 获取视频输入1  音频信息输入0;
void Mt_OpenVideo::AV_info(AVCodecParameters ** av, int avs)
{
	if (avs == 1)
	{
		AVCodecParameters * s = avcodec_parameters_alloc();
		if ( s == NULL)
		{
			avcodec_parameters_free(&s);
			return;
		}
		//拷贝信息
		avcodec_parameters_copy(s, pFormatCtx->streams[videoStream]->codecpar);
		*av = s;
	}
	else if (avs ==0)
	{
		AVCodecParameters * s = avcodec_parameters_alloc();
		if (s == NULL)
		{
			avcodec_parameters_free(&s);
			return;
		}
		//拷贝信息
		avcodec_parameters_copy(s, pFormatCtx->streams[audioStream]->codecpar);
		*av = s;
	}
}
//
bool Mt_OpenVideo::Seek(double pos)
{
	mux.lock();
	if (!pFormatCtx)
	{
		mux.unlock();
		return false;
	}
	//清理读取缓冲
	avformat_flush(pFormatCtx);
	long long seekPos = 0;
	seekPos = pFormatCtx->streams[videoStream]->duration * pos;
	std::cout << "seekPos:" <<seekPos<< std::endl;
	int re = av_seek_frame(pFormatCtx, videoStream, seekPos,
			AVSEEK_FLAG_BACKWARD | AVSEEK_FLAG_FRAME); //往后找关键帧
	//vpts = seekPos *r2d(pFormatCtx->streams[videoStream]->time_base) * 1000;
	mux.unlock();
	if (re < 0)
	{
		return false;
	}
	return true;
}



//清空读取缓存
void Mt_OpenVideo::ReadClear()
{
	if (!pFormatCtx)	
		return;
	//清理读取缓冲
	avformat_flush(pFormatCtx);
}

void Mt_OpenVideo::close()
{
	if (pFormatCtx)
	{
		avformat_close_input(&pFormatCtx);
	}

	if (vs)
	{
		avcodec_parameters_free(&vs);
	}
	if (as)
	{
		avcodec_parameters_free(&as);
	}
}
