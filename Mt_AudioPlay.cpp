#include "Mt_AudioPlay.h"




#if defined WIN32|| defined _WIN32
#include <windows.h>
#endif
//��ȡCPU����
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

//����Ƶ������
bool Mt_AudioPlay::open()
{
	QAudioFormat fmt;
	fmt.setSampleRate(sampleRate);		//����������
	fmt.setSampleSize(sampleSize);		//����ͨ����	
	fmt.setChannelCount(channels);		//����������С
	fmt.setCodec("audio/pcm");			//������Ƶ��ʽ
	fmt.setByteOrder(QAudioFormat::LittleEndian);	//����ΪС��
	fmt.setSampleType(QAudioFormat::UnSignedInt);		//����������ʽ
														//��ȡ���ǵ���Ƶ�豸 
	QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
	if (!info.isFormatSupported(fmt))
	{
		//std::cout << "�����豸��֧�֣�"<<"������������" << std::endl;
		fmt = info.nearestFormat(fmt);  //�Զ�����
	}

	output = new QAudioOutput(fmt);
	io = output->start();
	if (io)
	{
		return true;
	}

	//std::cout << "����Ƶ�������ɹ�!!" << std::endl;
	return false;
}


//��Ƶ����
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

//������Ƶ������ͣ
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
		output->suspend();  //��ͣ������Ƶ
	}
	else
	{
		output->resume();  //�ָ�������Ƶ
	}
	mux.unlock();
}

//���ػ����л�û���ŵ���Ƶʱ��(MS)
long long Mt_AudioPlay::GetNoPlayMs()
{
	if (!output)
		return 0;

	long long pts = 0;
	//���ػ�û�в��ŵ��ֽ���
	double size = output->bufferSize();  
	//����һ����Ƶ�ֽ����Ĵ�С
	double secSize = sampleRate *(sampleSize / 8)*channels;

	if (secSize <= 0)
		return 0;
	else
		return (size / secSize) * 1000;
}

bool Mt_AudioPlay::InitResample(AVCodecParameters *para)
{
	/// 1����Ƶ�ز��� �����ĳ�ʼ��
	asc = swr_alloc();
	asc = swr_alloc_set_opts(asc,
		av_get_default_channel_layout(2),  // Ϊ����������ͨ������Ĭ��ͨ������
		AV_SAMPLE_FMT_S16,	//�����ʽ
		para->sample_rate,//���������
		av_get_default_channel_layout(para->channels),  // Ϊ����������ͨ������Ĭ��ͨ������
		(AVSampleFormat)para->format,	//�����ʽ
		para->sample_rate,//����������
		0, 0
	);
// 	if (para)
// 		avcodec_parameters_free(&para); //�ͷſռ�

	if (!asc)
	{
		swr_free(&asc);
		//std::cout << "swr_alloc_set_opts() failed!" << std::endl;
		return false;
	}

	///2 ���ز�����ʼ��
	int ret = swr_init(asc);
	if (ret != 0)
	{
		//std::cout << "InitResample()" << std::endl;
		return false;
	}

	//std::cout << "��Ƶ����ز�����ʼ���ɹ���" << std::endl;
	return true;
}

bool Mt_AudioPlay::openDecode(AVCodecParameters * para)
{
	if (para == NULL)
		return false;

	//���ұ�����
	codec = avcodec_find_decoder(para->codec_id);
	if (codec == NULL)
	{
		avcodec_parameters_free(&para);
		//std::cout << "û���ҵ�������!" << std::endl;
		return false;
	}

	//������Ƶ������������
	ac = avcodec_alloc_context3(codec);

	if (ac == NULL)
	{
		//std::cout << "avcodec_alloc_context3  failed!" << std::endl;
		return false;
	}

	//�������ý����������Ĳ���
	avcodec_parameters_to_context(ac, para);

	std::cout << "find the AVCodec " << para->codec_id << std::endl;
	//ac->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
	ac->thread_count = GetCPU_num();

	//std::cout << "��Ƶ�����߳�����!!" << ac->thread_count << "����ȫ��!" << std::endl;
	int ret = avcodec_open2(ac, 0, 0);
	if (ret != 0)
	{
		std::cout << "avconde_open2() audio failed!" << std::endl;
	}

	std::cout << "vcodec_open2 success!" << std::endl;
	return true;
}

//���͵������߳�,���ܳɹ�����ʧ�ܶ�����pkt
bool Mt_AudioPlay::send(AVPacket *pkt)
{
	//�ݴ���
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

//��ȡ�������ݣ�һ��send������Ҫ���Recv����ȡ�����е�����Send NULL��Recv���
//ÿ�θ���һ�ݣ��ɵ������ͷ� av_frame_free
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


//��Ƶ�ز���
int Mt_AudioPlay::AudioResamle(AVFrame *indata, unsigned char * adata)
{
	if (!indata)
		//av_frame_free(&indata);
		return -1;

	if (!adata)
		av_frame_free(&indata);    //�ͷ��ڴ�

	uint8_t *data[2] = { 0 };
	data[0] = adata;

	//��Ƶ�ز���
	int  re = swr_convert(asc, data, indata->nb_samples,  //���
		(const uint8_t**)indata->data, indata->nb_samples);//����
	
	//������Ƶ���ݴ�С
	int outSize = re * indata->channels *av_get_bytes_per_sample((AVSampleFormat)outFmt);	
	av_frame_free(&indata);
	if (re <= 0)
		return re;
	return outSize;
}

//������Ƶ��û�в��ŵ��ֽ���
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
//������Ƶ����
void Mt_AudioPlay::AudioClear()
{
	if (io)
	{
		io->reset();
	}

	if (ac)
	{
		avcodec_flush_buffers(ac); //�����Ƶ���뻺��
	}
}

