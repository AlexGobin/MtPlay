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

bool Mt_VideoThread::isexit = true;

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

	while (isexit)
	{
		if (this->isPress)
		{
			QThread::msleep(5);
			continue;
		}
	
		if (VidoeQueue.empty())
		{			
			QThread::msleep(3);
			continue;
		}

		pkt = VidoeQueue.front();
		VidoeQueue.pop();

		DE->send(pkt);    //解码
		//vpts = frame->pts * av_q2d(avs->time_base) ;
		//std::cout << "vpts:" << vpts<<std::endl;
		DE->recv(&frame);			
		if (!frame)		
			continue;
		vpts =  (av_q2d(avs->time_base)) * frame->pts  * 1000;	
		while (vpts > apts)
		{
			QThread::msleep(5);
		}		
		VidoeAVF.push(frame);
		emit show();

		//QThread::msleep(1000 / fps - );
			
	}		
	ThreadClear();
}

void Mt_VideoThread::set(bool isexit)
{
	this->isexit = isexit;
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

