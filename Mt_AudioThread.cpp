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
	std::cout <<" ----------------��Ƶ�����߳�����! -------------------"<< std::endl;
	unsigned char *pcm = new unsigned char[1024*1024*10];
	if (!pcm)	
		return;
	
	AVPacket * pkt = NULL;
	AVFrame  * frame = NULL;

	if (!audio->open()) //����Ƶ������
	{
		//std::cout << "����Ƶ������ʧ��!" << std::endl;
		return;
	}
			
	if (!audio->openDecode(as)) 
	{
		//std::cout << "����Ƶ������ʧ��!" << std::endl;
		return;
	}

	//��Ƶ�ز�����ʼ��
	if (!audio->InitResample(as))
	{
		//std::cout << "�ز�����ʼ��ʧ��!" << std::endl;
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
		
		while (audio->recv(&frame))
		{		
			//std::cout << "frame->apts:" << frame->pts << std::endl;
			Apts = frame->pts;
			apts = audio->apts - audio->GetNoPlayMs();//����PTS ��ȥ������Ϊ���ŵ�ʱ��	
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
				audio->play(pcm, size);  //������Ƶ
				break;
			}			
		}
	}			
	
	ThreadClear();
	delete pcm;
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

