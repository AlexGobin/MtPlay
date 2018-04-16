#ifndef  __MT_VIDEODECODE_H__ 
#define	 __MT_VIDEODECODE_H__

#include "Mt_deque.h"


class Mt_VideoDecode
{
public:
	Mt_VideoDecode();
	~Mt_VideoDecode();

public:
	//��Ƶ������
	AVCodec *vcodec = NULL;
	//�����PTS
	long long pts = 0;

public:
	//����Ƶ������
	bool open(AVCodecParameters *para);

	//���͵������߳�,���ܳɹ�����ʧ�ܶ�����pkt
	bool send(AVPacket *pkt);

	//��ȡ�������ݣ�һ��send������Ҫ���Recv����ȡ�����е�����Send NULL��Recv���
	//ÿ�θ���һ�ݣ��ɵ������ͷ� av_frame_free
	bool recv(AVFrame ** frame);

	//������뻺��
	void DecodeClear();

	//��������ڴ�.
	void close();
protected:
	AVCodecContext *codec = NULL;
	std::mutex mux;
};

#endif
