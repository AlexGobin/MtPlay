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
#include <libswresample/swresample.h>  //音频重采样
}

//编码解码
#pragma  comment(lib,"avcodec.lib")
//视频打开类  文件格式
#pragma comment(lib,"avformat.lib")
//转码类 编码
#pragma comment(lib,"swscale.lib")

#pragma comment(lib,"swresample.lib")
//工具类
#pragma comment(lib,"avutil.lib")

extern long long apts;  //音频PTS
extern long long vpts;  //视频PTS
extern long long Apts;

extern AVStream *avs;

extern HFYqueue<AVPacket*> AudioQueue;
extern HFYqueue<AVPacket*> VideoQueue;

extern HFYqueue<AVFrame*> VidoeAVF;


//队列
// extern std::queue<AVPacket*> AudioQueue;	//音频队列
// extern std::queue<AVPacket*> VidoeQueue;	//视频队列
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