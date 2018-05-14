#include "Mt_VideoDecode.h"



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

Mt_VideoDecode::Mt_VideoDecode()
{
}


Mt_VideoDecode::~Mt_VideoDecode()
{
}

bool Mt_VideoDecode::open(AVCodecParameters *para)
{
	if (para == NULL)
		return false;

	//���ұ�����
	vcodec = avcodec_find_decoder(para->codec_id);
	if (vcodec == NULL)
	{
		avcodec_parameters_free(&para);
		//std::cout << "û���ҵ�������!" << std::endl;
		return false;
	}
	std::cout << "find the AVCodec " << para->codec_id << std::endl;
	
	//���ý�����������
	codec = avcodec_alloc_context3(vcodec);
	avcodec_parameters_to_context(codec, para);
	
	int x = GetCPU_num();
	//std::cout << "��ĵ��Ե�CPU�߳���:" << x << "����ȫ��" << std::endl;
	codec->thread_count = GetCPU_num(); //��ȡCPU�߳����� ����ȫ��

	//�򿪽�����������
	int re = avcodec_open2(codec, 0, 0);
	if (re != 0)
	{
		avcodec_free_context(&codec);
		std::cout << "avcodec_open2() failed!" << std::endl;
		return false;
	}
	std::cout << "avcodec_open2() success!" << std::endl;
	return true;
}

//���͵������߳�,���ܳɹ�����ʧ�ܶ�����pkt
bool Mt_VideoDecode::send(AVPacket *pkt)
{
	//�ݴ���
	if (!pkt || pkt->size <= 0 || !pkt->data)return false;
	if (!codec)
	{
		return false;
	}
	int re = avcodec_send_packet(codec, pkt);
	av_packet_free(&pkt);
	if (re != 0)return false;
	return true;
}

bool Mt_VideoDecode::recv(AVFrame ** frame)
{
	if (!codec)	
		return false;	
	AVFrame * temp = av_frame_alloc();
	int re = avcodec_receive_frame(codec, temp);
	if (re != 0)
	{
		av_frame_free(&temp);
		return false;
	}
	pts = temp->pts;
	*frame = temp;
	return true;
}

//������뻺��
void Mt_VideoDecode::DecodeClear()
{
	if (codec)
	{
		avcodec_flush_buffers(codec);	
	}
}

void Mt_VideoDecode::close()
{
	if (codec)
	{
		avcodec_free_context(&codec);
	}
	

}
