#ifndef __MT_OPENVIDEO_H__
#define __MT_OPENVIDEO_H__

#include "Mt_deque.h"

class Mt_OpenVideo
{
public:
	Mt_OpenVideo();
	~Mt_OpenVideo();

public:
	int totalMs = -1;		//��Ƶ��ʱ��
	int width = 0;			//��Ƶ��
	int height = 0;			//��Ƶ��
	int fps = 0;

	int videoStream = -1;	//��Ƶ�������
	int audioStream = -2;	//��Ƶ�������

							//��Ƶ�������
	int sampleRate = 0;     //������
	int channles = 0;		//ͨ����
	int sampleByte = 0;     //������С

public:
	//�洢��Ƶ�ļ���Ϣ�ṹ�� ���װ
	AVFormatContext	* pFormatCtx = NULL;
	//��ȡ��Ƶ����Ƶ��Ϣ
	AVCodecParameters * AV = NULL;
	//
	std::mutex mux;

public:
	//���ļ�
	int open(const char * filename);

	//��ȡ��Ƶ
	int read(AVPacket **pkt);

	//����Ƶ������
	bool AVSeparate(AVPacket *pkt);

	//��ȡ��Ƶ����Ƶ��Ϣ ��ȡ��Ƶ����1  ��Ƶ��Ϣ����0;
	void AV_info(AVCodecParameters ** av, int avs);

	//��ȡ��Ƶλ��
	bool Seek(double pos);

	//�����ȡ����
	void ReadClear();

	//��������ڴ�
	void close();
};

#endif