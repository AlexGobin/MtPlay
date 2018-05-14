#include "Mt_ReadThread.h"

Mt_ReadThread::Mt_ReadThread(QObject *parent)
	: QObject(parent)
{
	if (IO == NULL)
	{
		IO = new Mt_OpenVideo;
	}
}

Mt_ReadThread::~Mt_ReadThread()
{
	delete IO;
}

bool Mt_ReadThread::isExit = true;

bool Mt_ReadThread::isPress = false;

//�̴߳�����
void Mt_ReadThread::run(QString filename)
{

	//std::cout << "------------��ȡ�߳�����!-----------------" << std::endl;
	//this->queueClear();		//�����ڴ�����
	close();
	int ret = IO->open(filename.toLocal8Bit());
	if (ret != 0)
		return;


	IO->AV_info(&as, 0);
	IO->AV_info(&vs, 1);
	
	int width = IO->width;
	int height = IO->height;
	int fps = IO->fps;
	AVPacket * pkt = NULL;
// 	std::cout << "��Ƶ��:" << width << std::endl;
// 	std::cout << "��Ƶ��:" << height << std::endl;
	emit Initshow(width, height,IO->videoFMt);			//��Ƶ��ʾ�ź�
	emit AudioStart();						//��Ƶ�����ź�
	emit VideoStart(fps);					//��Ƶ�����ź�
	emit VideoTime(IO->totalMs);            //������Ƶ��ʱ��

	while (isExit)
	{
		//std::cout << "��ʼ������!!!!!" << std::endl;
		if (this->isPress)
		{
			QThread::msleep(2);
			continue;
		}
		while ((VideoQueue.size() > 30 || AudioQueue.size() > 30) && isExit != false ) 
		{					
			QThread::msleep(2);
			continue;
		}
		int ret = IO->read(&pkt);  //��ȡ��Ƶ
		if (ret != 0)
		{		
			QThread::msleep(1);
			continue;
		}
		//��������Ƶ		
		IO->AVSeparate(pkt);		
		QThread::msleep(5);

	}
	ThreadClear();	
	this->close();
	
	std::cout << "read��ȡ�߳̽���!" << std::endl;
}

void Mt_ReadThread::set(bool isexit)
{
	this->isExit = isexit;
}


void Mt_ReadThread::ThreadPress(bool isPress)
{
	this->isPress = isPress;
}

void Mt_ReadThread::close()
{
	IO->close();
}

void Mt_ReadThread::seek(double pos)
{

	//IO->ReadClear();  //�����ȡ����
	IO->Seek(pos);
}


//�����̻߳���
void Mt_ReadThread::ThreadClear()
{
	IO->ReadClear();
}

void Mt_ReadThread::queueClear()
{
	this->aPktQueueClear();
	this->vPktQueueClear();
}

void Mt_ReadThread::vPktQueueClear()
{

	while(VideoQueue.empty() != true)
	{
		AVPacket* pkt = AudioQueue.pop();
		av_packet_free(&pkt);
		
	}
}

void Mt_ReadThread::aPktQueueClear()
{
	while (AudioQueue.empty() != true)
	{
		AVPacket* pkt = AudioQueue.pop();
		av_packet_free(&pkt);
		
	}
}

void Mt_ReadThread::vFrameQueueClear()
{
	while(VidoeAVF.empty() != true)
	{
		AVFrame * frame = VidoeAVF.pop();
		av_frame_free(&frame);
		frame = NULL;
	}
}

