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

//��Ƶ�����̲߳��ź���
void Mt_AudioThread::run()
{
	//std::cout <<" ----------------��Ƶ�����߳�����! -------------------"<< std::endl;
	unsigned char *pcm = new unsigned char[1024*1024*10];
	if (!pcm)	
		return;
	
	AVPacket * pkt = NULL;
	AVFrame  * frame = NULL;

	if (!audio->open()) //����Ƶ������
	{
		return;
	}
			
	if (!audio->openDecode(as)) 
	{
		return;
	}

	//��Ƶ�ز�����ʼ��
	if (!audio->InitResample(as))
	{	
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
	
		audio->send(pkt); //���͵������߳�	
		
		while (isExit)
		{					
			if (!audio->recv(&frame))
			{
				break;
			}
			Apts = frame->pts;
			apts = audio->apts - audio->GetNoPlayMs();//����PTS ��ȥ������Ϊ���ŵ�ʱ��	
			//std::cout << "apts:" << apts <<std::endl;
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
				
				audio->play(pcm, size);  //������Ƶ
				break;
			}			
		}
	}			
	
	ThreadClear();
	audio->close();
	delete [] pcm;
	std::cout << "audio�߳̽���!" << std::endl;

}

//�����߳�ѭ��ֹͣ
void Mt_AudioThread::set(bool isexit)
{
	this->isExit = isexit;
}


//��ͣ����
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

