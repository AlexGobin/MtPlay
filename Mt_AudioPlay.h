#ifndef	__MT_AUDIOPLAY_H__
#define __MT_AUDIOPLAY_H__

#include <QAudioOutput>
#include <QAudioFormat>
#include "Mt_deque.h"


enum SampleFMT
{
	X_S16 = 1,
	X_FLATP = 8
};

class Mt_AudioPlay
{
public:
	Mt_AudioPlay();
	~Mt_AudioPlay();

public:
	int sampleRate = 44100;		//������
	int sampleSize = 16;		//������С
	int channels = 2;			//ͨ����
	long long apts = 0;			//��ƵPTS

	QAudioOutput *output = NULL;                    //��Ƶ����
	QIODevice *io = NULL;
	

	//��Ƶ
	SwrContext * asc = NULL;		//��Ƶ�ز���������
	SampleFMT outFmt = X_S16;		//��Ƶ�����ʽ
	//SampleFMT outFmt = X_FLATP;	//��Ƶ�����ʽ
	AVCodecContext * ac = NULL;     //��Ƶ����
	AVCodec *codec = NULL;          //��Ƶ������
public:
	//����Ƶ������
	bool open();

	//������Ƶ
	bool play(const unsigned char *data, int datasize);

	//���ò��Ż���ͣ
	void setPause(bool isPause);

	//���ػ����л�û�в��ŵ�ʱ��(����)
	long long GetNoPlayMs();

	//��Ƶ�ز�����ʼ��
	bool InitResample(AVCodecParameters *para);

	//����Ƶ������
	bool openDecode(AVCodecParameters * para);

	//���͵������߳�,���ܳɹ�����ʧ�ܶ�����pkt
	bool send(AVPacket *pkt);

	//��ȡ�������ݣ�һ��send������Ҫ���Recv����ȡ�����е�����Send NULL��Recv���
	//ÿ�θ���һ�ݣ��ɵ������ͷ� av_frame_free
	bool recv(AVFrame ** frame);

	//��Ƶ�ز��� �����ز�����С
	int AudioResamle(AVFrame *indata,unsigned char * adata);

	//������Ƶ��û�в��ŵ��ֽ���
	int GetFree();

	//������Ƶ����
	void AudioClear();

	//�����ڴ�
	void close();


public:
	std::mutex mux;


};


#endif
