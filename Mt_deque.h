#ifndef  __MT_DEQUE_H__
#define	 __MT_DEQUE_H__

#include <deque>
#include <mutex>
#include <iostream>
#include <thread>
#include <QThread>
#include <QString>
#include <queue>

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libswresample/swresample.h>  //��Ƶ�ز���
}

//�������
#pragma  comment(lib,"avcodec.lib")
//��Ƶ����  �ļ���ʽ
#pragma comment(lib,"avformat.lib")
//ת���� ����
#pragma comment(lib,"swscale.lib")

#pragma comment(lib,"swresample.lib")
//������
#pragma comment(lib,"avutil.lib")

extern long long apts;  //��ƵPTS
extern long long vpts;  //��ƵPTS

extern AVStream *avs;
//����
extern std::queue<AVPacket*> AudioQueue;	//��Ƶ����
extern std::queue<AVPacket*> VidoeQueue;	//��Ƶ����


extern std::queue<AVFrame *> VidoeAVF;

extern AVCodecParameters *vs ;
extern AVCodecParameters *as ;

#endif