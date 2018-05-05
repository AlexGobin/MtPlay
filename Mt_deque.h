#ifndef  __MT_DEQUE_H__
#define	 __MT_DEQUE_H__

#include <deque>
#include <mutex>
#include <iostream>
#include <thread>
#include <QThread>
#include <QString>
#include <queue>
#include "HFYqueue.hpp"

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
extern long long Apts;

extern AVStream *avs;

extern HFYqueue<AVPacket*> AudioQueue;
extern HFYqueue<AVPacket*> VideoQueue;

extern HFYqueue<AVFrame*> VidoeAVF;


//����
// extern std::queue<AVPacket*> AudioQueue;	//��Ƶ����
// extern std::queue<AVPacket*> VidoeQueue;	//��Ƶ����
// 
// extern std::queue<AVFrame *> VidoeAVF;

extern AVCodecParameters *vs ;
extern AVCodecParameters *as ;

// enum AVPixelFormat {
// 	AV_PIX_FMT_YUV420P =0,   ///< planar YUV 4:2:0, 12bpp, (1 Cr & Cb sample per 2x2 Y samples)
// 	AV_PIX_FMT_YUV422P =4,   ///< planar YUV 4:2:2, 16bpp, (1 Cr & Cb sample per 2x1 Y samples)
// 	AV_PIX_FMT_YUV444P =5,  ///< planar YUV 4:4:4, 24bpp, (1 Cr & Cb sample per 1x1 Y samples)
// };


#endif