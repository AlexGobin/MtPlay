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
	int sampleRate = 44100;		//样本率
	int sampleSize = 16;		//样本大小
	int channels = 2;			//通道数
	long long apts = 0;			//音频PTS

	QAudioOutput *output = NULL;                    //音频播放
	QIODevice *io = NULL;
	

	//音频
	SwrContext * asc = NULL;		//音频重采样上下文
	SampleFMT outFmt = X_S16;		//音频输入格式
	//SampleFMT outFmt = X_FLATP;	//音频输出格式
	AVCodecContext * ac = NULL;     //音频编码
	AVCodec *codec = NULL;          //音频解码器
public:
	//打开音频播放器
	bool open();

	//播放音频
	bool play(const unsigned char *data, int datasize);

	//设置播放或暂停
	void setPause(bool isPause);

	//返回缓冲中还没有播放的时间(毫秒)
	long long GetNoPlayMs();

	//音频重采样初始化
	bool InitResample(AVCodecParameters *para);

	//打开音频解码器
	bool openDecode(AVCodecParameters * para);

	//发送到解码线程,不管成功还是失败都清理pkt
	bool send(AVPacket *pkt);

	//获取解码数据，一次send可能需要多次Recv，获取缓冲中的数据Send NULL在Recv多次
	//每次复制一份，由调用者释放 av_frame_free
	bool recv(AVFrame ** frame);

	//音频重采样 返回重采样大小
	int AudioResamle(AVFrame *indata,unsigned char * adata);

	//返回音频中没有播放的字节数
	int GetFree();

	//清理音频缓冲
	void AudioClear();

	//回收内存
	void close();


public:
	std::mutex mux;


};


#endif
