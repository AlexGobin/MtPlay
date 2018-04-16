#ifndef __MT_OPENVIDEO_H__
#define __MT_OPENVIDEO_H__

#include "Mt_deque.h"

class Mt_OpenVideo
{
public:
	Mt_OpenVideo();
	~Mt_OpenVideo();

public:
	int totalMs = -1;		//视频总时长
	int width = 0;			//视频宽
	int height = 0;			//视频高
	int fps = 0;

	int videoStream = -1;	//视频索引编号
	int audioStream = -2;	//音频索引编号

							//音频输入参数
	int sampleRate = 0;     //样本率
	int channles = 0;		//通道数
	int sampleByte = 0;     //样本大小

public:
	//存储视频文件信息结构体 解封装
	AVFormatContext	* pFormatCtx = NULL;
	//获取视频或音频信息
	AVCodecParameters * AV = NULL;
	//
	std::mutex mux;

public:
	//打开文件
	int open(const char * filename);

	//读取视频
	int read(AVPacket **pkt);

	//音视频分离器
	bool AVSeparate(AVPacket *pkt);

	//获取视频或音频信息 获取视频输入1  音频信息输入0;
	void AV_info(AVCodecParameters ** av, int avs);

	//获取视频位置
	bool Seek(double pos);

	//清理读取缓冲
	void ReadClear();

	//清理回收内存
	void close();
};

#endif