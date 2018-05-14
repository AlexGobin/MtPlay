#include "Mt_OpenVideo.h"

// pts ʱ�任��  ת���ɺ���
static double r2d(AVRational r)
{
	return r.num == 0 || r.den == 0 ? 0. : (double)r.num / (double)r.den;
}

Mt_OpenVideo::Mt_OpenVideo()
{
	static bool isFrist = true;
	if (isFrist)
	{
		av_register_all();         //��ʼ��
		avformat_network_init();		
		avcodec_register_all();//ע�������
		isFrist = false;
	}
}


Mt_OpenVideo::~Mt_OpenVideo()
{
	close();
}

//����Ƶ�ļ�
int Mt_OpenVideo::open(const char * filename)
{
	close();
	////����ռ�
	//pFormatCtx = avformat_alloc_context();
	//if (pFormatCtx == NULL)
	//{
	//	std::cout << "avformat_alloc_context() failed!" << std::endl;
	//	return -1;
	//}

	AVDictionary * opts = NULL;
	//����rtsp����tcpЭ���
	av_dict_set(&opts, "rtsp_transport", "tcp", 0);
	//������ʱʱ�� rtsp ���÷ǳ���Ҫ
	av_dict_set(&opts, "max_delay", "500", 0);

	//����Ƶ�ļ�
	int ret = avformat_open_input(&pFormatCtx, filename, NULL, &opts);
	if (ret != 0)
	{
		std::cout << "avformat_open_input() failed!" << std::endl;
		return -1;
	}

	//�ٴλ�ȡ����Ϣ 
	ret = avformat_find_stream_info(pFormatCtx, NULL);
	if (ret < 0)
	{
		std::cout << "avformat_find_stream_info() failed" << std::endl;
		return -2;
	}

	//��ӡ��Ƶ�ļ���Ϣ
	av_dump_format(pFormatCtx, 0, filename, 0);
	//��ȡ��Ƶ��ʱ��
	this->totalMs = (pFormatCtx->duration / AV_TIME_BASE) * 1000;
	std::cout << "��Ƶ��ʱ��" << totalMs << std::endl;
	

	//��ȡ��Ƶ
	videoStream = av_find_best_stream(pFormatCtx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
	avs = pFormatCtx->streams[videoStream];
	this->width = avs->codecpar->width;    //��
	this->height = avs->codecpar->height;	//��
	this->fps = r2d(avs->avg_frame_rate);  //fps
	this->videoFMt = (int)pFormatCtx->streams[videoStream]->codec->pix_fmt;

	//��ȡ��Ƶ
	audioStream = av_find_best_stream(pFormatCtx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
	//avs = pFormatCtx->streams[audioStream];
	this->channles = avs->codecpar->channels;    //ͨ����
	this->sampleRate = avs->codecpar->sample_rate;  //������
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
	
	//����pts dts
	packet->pts = packet->pts*(1000 * (r2d(pFormatCtx->streams[packet->stream_index]->time_base)));
	packet->dts = packet->dts*(1000 * (r2d(pFormatCtx->streams[packet->stream_index]->time_base)));

	//std::cout << "pts:" << packet->pts << "-----" << "dts:" << packet->dts << std::endl;	
	*pkt = packet;
	return 0;
}

//����Ƶ������
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

//��ȡ��Ƶ����Ƶ��Ϣ ��ȡ��Ƶ����1  ��Ƶ��Ϣ����0;
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
		//������Ϣ
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
		//������Ϣ
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
	//�����ȡ����
	avformat_flush(pFormatCtx);
	long long seekPos = 0;
	seekPos = pFormatCtx->streams[videoStream]->duration * pos;
	std::cout << "seekPos:" <<seekPos<< std::endl;
	int re = av_seek_frame(pFormatCtx, videoStream, seekPos,
			AVSEEK_FLAG_BACKWARD | AVSEEK_FLAG_FRAME); //�����ҹؼ�֡
	//vpts = seekPos *r2d(pFormatCtx->streams[videoStream]->time_base) * 1000;
	mux.unlock();
	if (re < 0)
	{
		return false;
	}
	return true;
}



//��ն�ȡ����
void Mt_OpenVideo::ReadClear()
{
	if (!pFormatCtx)	
		return;
	//�����ȡ����
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
