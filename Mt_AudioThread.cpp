#include "Mt_AudioThread.h"

Mt_AudioThread::Mt_AudioThread(QObject *parent)
	: QObject(parent)
{
	if (audio == NULL)
	{
		audio = new Mt_AudioPlay;
	}	
}

Mt_AudioThread::~Mt_AudioThread()
{
	delete audio;
}


bool Mt_AudioThread::isExit = true;
bool Mt_AudioThread::isPause = false;

//音频解码线程播放函数
void Mt_AudioThread::run()
{
	std::cout <<" ----------------音频解码线程启动! -------------------"<< std::endl;
	unsigned char *pcm = new unsigned char[1024*1024*10];
	if (!pcm)	
		return;
	
	AVPacket * pkt = NULL;
	AVFrame  * frame = NULL;

	if (!audio->open()) //打开音频播放器
	{
		//std::cout << "打开音频播放器失败!" << std::endl;
		return;
	}
			
	if (!audio->openDecode(as)) 
	{
		//std::cout << "打开音频解码器失败!" << std::endl;
		return;
	}

	//音频重采样初始化
	if (!audio->InitResample(as))
	{
		//std::cout << "重采样初始化失败!" << std::endl;
		return;
	}
	
	while(isExit)
	{		
		if (isPause)
		{				
			QThread::msleep(5);
			continue;
		}
		if (AudioQueue.empty())
		{			
			QThread::msleep(2);
			continue;
		}

		pkt = AudioQueue.pop();
	
		audio->send(pkt); //发送到解码线程	
		
		while (audio->recv(&frame))
		{		
			//std::cout << "frame->apts:" << frame->pts << std::endl;
			Apts = frame->pts;
			apts = audio->apts - audio->GetNoPlayMs();//计算PTS 减去缓冲中为播放的时间	
		//	std::cout << "apts:" << apts <<std::endl;
			int size = audio->AudioResamle(frame, pcm);
			while (isExit)
			{
				if (size <= 0)
					break;
				if (audio->GetFree() < size || isPause)
				{
					QThread::msleep(2);
					continue;
				}
				audio->play(pcm, size);  //播放音频
				break;
			}			
		}
	}			
	
	ThreadClear();
	delete pcm;
}

//设置线程循环停止
void Mt_AudioThread::set(bool isexit)
{
	this->isExit = isexit;
}


//暂停播放
void Mt_AudioThread::ThreadPress(bool isPause)
{
	this->isPause = isPause;
}

//
void Mt_AudioThread::ThreadClear()
{
	if (audio)
	{
		audio->AudioClear();
	}
}

