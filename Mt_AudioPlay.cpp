#include "Mt_AudioPlay.h"




#if defined WIN32|| defined _WIN32
#include <windows.h>
#endif
//获取CPU数量
static int GetCPU_num()
{
#if defined WIN32 || defined _WIN32
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	return (int)sysinfo.dwNumberOfProcessors;  //windows

#elif defined __linux__
	return (int)sysconf(_SC_NPROCESSORS_ONLN);  //linux

#elif defined __APPLE__
	int numCPU = 0;
	int mib[4];
	size_t len = sizeof(numCPU);
	mib[0] = CTL_HW;
	mib[1] = HW_AVAILCPU;
	sysctl(mib, 2, &numCPU, &len, NULL, 0);
	if (numCPU < 1)
	{
		mib[1] = HW_NCPU;
		sysctl(mib, 2, &numCPU, &len, NULL, 0);
		if (numCPU < 1)
		{
			numCPU = 1;
		}
	}
	return (int)numCPU;
#else
	return 1;
#endif
}

Mt_AudioPlay::Mt_AudioPlay()
{
}


Mt_AudioPlay::~Mt_AudioPlay()
{
}

//打开音频播放器
bool Mt_AudioPlay::open()
{
	QAudioFormat fmt;
	fmt.setSampleRate(sampleRate);		//设置样本率
	fmt.setSampleSize(sampleSize);		//设置通道数	
	fmt.setChannelCount(channels);		//设置样本大小
	fmt.setCodec("audio/pcm");			//设置音频格式
	fmt.setByteOrder(QAudioFormat::LittleEndian);	//设置为小端
	fmt.setSampleType(QAudioFormat::UnSignedInt);		//设置样本格式
														//获取我们的音频设备 
	QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
	if (!info.isFormatSupported(fmt))
	{
		//std::cout << "声卡设备不支持！"<<"重新设置音质" << std::endl;
		fmt = info.nearestFormat(fmt);  //自动设置
	}

	output = new QAudioOutput(fmt);
	io = output->start();
	if (io)
	{
		return true;
	}

	//std::cout << "打开音频播放器成功!!" << std::endl;
	return false;
}


//音频播放
bool Mt_AudioPlay::play(const unsigned char *data, int datasize)
{
	if (!data || datasize <= 0)
		return false;

	if (!output || !io)
		return false;

	int size = io->write((char *)data, datasize);
	if (datasize != size)	
		return false;

	return true;
}

//设置音频播放暂停
void Mt_AudioPlay::setPause(bool isPause)
{
	mux.lock();
	if (!output)
	{
		mux.unlock();
		return;
	}
	
	if (!isPause)
	{
		output->suspend();  //暂停播放音频
	}
	else
	{
		output->resume();  //恢复播放音频
	}
	mux.unlock();
}

//返回缓冲中还没播放的音频时间(MS)
long long Mt_AudioPlay::GetNoPlayMs()
{
	if (!output)
		return 0;

	long long pts = 0;
	//返回还没有播放的字节数
	double size = output->bufferSize();  
	//计算一秒音频字节数的大小
	double secSize = sampleRate *(sampleSize / 8)*channels;

	if (secSize <= 0)
		return 0;
	else
		return (size / secSize) * 1000;
}

bool Mt_AudioPlay::InitResample(AVCodecParameters *para)
{
	/// 1、音频重采样 上下文初始化
	asc = swr_alloc();
	asc = swr_alloc_set_opts(asc,
		av_get_default_channel_layout(2),  // 为给定数量的通道返回默认通道类型
		AV_SAMPLE_FMT_S16,	//输出格式
		para->sample_rate,//输出样本率
		av_get_default_channel_layout(para->channels),  // 为给定数量的通道返回默认通道类型
		(AVSampleFormat)para->format,	//输入格式
		para->sample_rate,//输入样本率
		0, 0
	);
// 	if (para)
// 		avcodec_parameters_free(&para); //释放空间

	if (!asc)
	{
		swr_free(&asc);
		//std::cout << "swr_alloc_set_opts() failed!" << std::endl;
		return false;
	}

	///2 、重采样初始化
	int ret = swr_init(asc);
	if (ret != 0)
	{
		//std::cout << "InitResample()" << std::endl;
		return false;
	}

	//std::cout << "音频输出重采样初始化成功！" << std::endl;
	return true;
}

bool Mt_AudioPlay::openDecode(AVCodecParameters * para)
{
	if (para == NULL)
		return false;

	//查找编码器
	codec = avcodec_find_decoder(para->codec_id);
	if (codec == NULL)
	{
		avcodec_parameters_free(&para);
		//std::cout << "没有找到解码器!" << std::endl;
		return false;
	}

	//创建音频编码器上下文
	ac = avcodec_alloc_context3(codec);

	if (ac == NULL)
	{
		//std::cout << "avcodec_alloc_context3  failed!" << std::endl;
		return false;
	}

	//配置配置解码器上下文参数
	avcodec_parameters_to_context(ac, para);

	std::cout << "find the AVCodec " << para->codec_id << std::endl;
	//ac->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
	ac->thread_count = GetCPU_num();

	//std::cout << "音频解码线程启动!!" << ac->thread_count << "火力全开!" << std::endl;
	int ret = avcodec_open2(ac, 0, 0);
	if (ret != 0)
	{
		std::cout << "avconde_open2() audio failed!" << std::endl;
	}

	std::cout << "vcodec_open2 success!" << std::endl;
	return true;
}

//发送到解码线程,不管成功还是失败都清理pkt
bool Mt_AudioPlay::send(AVPacket *pkt)
{
	//容错处理
	if (!pkt || pkt->size <= 0 || !pkt->data)return false;
	if (!codec)
	{
		mux.unlock();
		return false;
	}
	int re = avcodec_send_packet(ac, pkt);
	av_packet_free(&pkt);
	if (re != 0)return false;
	return true;
}

//获取解码数据，一次send可能需要多次Recv，获取缓冲中的数据Send NULL在Recv多次
//每次复制一份，由调用者释放 av_frame_free
bool Mt_AudioPlay::recv(AVFrame ** frame)
{
	if (!codec)
		return false;
	AVFrame * temp = av_frame_alloc();
	int re = avcodec_receive_frame(ac, temp);
	if (re != 0)
	{
		av_frame_free(&temp);
		return false;
	}
	apts = temp->pts;
	*frame = temp;
	return true;
}


//音频重采样
int Mt_AudioPlay::AudioResamle(AVFrame *indata, unsigned char * adata)
{
	if (!indata)
		//av_frame_free(&indata);
		return -1;

	if (!adata)
		av_frame_free(&indata);    //释放内存

	uint8_t *data[2] = { 0 };
	data[0] = adata;

	//音频重采样
	int  re = swr_convert(asc, data, indata->nb_samples,  //输出
		(const uint8_t**)indata->data, indata->nb_samples);//输入
	
	//计算音频数据大小
	int outSize = re * indata->channels *av_get_bytes_per_sample((AVSampleFormat)outFmt);	
	av_frame_free(&indata);
	if (re <= 0)
		return re;
	return outSize;
}

//返回音频中没有播放的字节数
int Mt_AudioPlay::GetFree()
{
	mux.lock();
	if (!output)
	{
		mux.unlock();
		return 0;
	}
	int free = output->bytesFree();
	mux.unlock();
	return free;
}
//清理音频缓冲
void Mt_AudioPlay::AudioClear()
{
	if (io)
	{
		io->reset();
	}

	if (ac)
	{
		avcodec_flush_buffers(ac); //清除音频解码缓冲
	}
}

