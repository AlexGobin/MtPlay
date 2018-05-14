#include "Mt_VideoThread.h"
#include <iostream>

// pts 时间换算  转换成毫秒
static double r2d(AVRational r)
{
	return r.num == 0 || r.den == 0 ? 0. : (double)r.num / (double)r.den;
}


Mt_VideoThread::Mt_VideoThread(QObject *parent)
	: QObject(parent)
{
	
	if (DE == NULL)
	{
		DE = new Mt_VideoDecode;
	}
}

Mt_VideoThread::~Mt_VideoThread()
{
	
	delete DE;
}

bool Mt_VideoThread::isExit = true;

bool Mt_VideoThread::isPress = false;

void Mt_VideoThread::run(int fps)
{
	//std::cout << "---------------视频解码线程启动!--------------" << std::endl;
	AVPacket * pkt = NULL;
	AVFrame  * frame = NULL;

	if (!DE->open(vs))
	{
		return;
		//std::cout << "视频解码器打开失败!!" << std::endl;
	}
	QThread::msleep(1);

	while (isExit)
	{
		
		if (this->isPress)
		{
			QThread::msleep(2);
			continue;
		}
	
		if ( VideoQueue.empty() )
		{			
			QThread::msleep(2);
			continue;
		}
	
		pkt = VideoQueue.pop();
	
		DE->send(pkt);    //解码
		
		DE->recv(&frame);			
		if (!frame)		
			continue;
		vpts = frame->pts;
		while ( vpts > Apts && isExit != false )		//视频同步音频
		{
			QThread::msleep(1);
			continue;
		}
		VidoeAVF.push(frame);
		emit show();
	}		
	
	ThreadClear();
	DE->close();
	std::cout << "video解码线程结束1" << std::endl;
}

void Mt_VideoThread::set(bool isexit)
{
	this->isExit = isexit;
}

//线程暂停函数
void Mt_VideoThread::ThreadPress(bool isPress)
{
	this->isPress = isPress;
}

//线程缓存清理
void Mt_VideoThread::ThreadClear()
{
	DE->DecodeClear();
}

