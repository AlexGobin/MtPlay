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

//线程处理函数
void Mt_ReadThread::run(QString filename)
{

	//std::cout << "------------读取线程启动!-----------------" << std::endl;

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

	emit Initshow(width, height);	//视频显示信号
// 	std::cout << "视频宽:" << width << std::endl;
// 	std::cout << "视频高:" << height << std::endl;
	emit AudioStart();						//音频启动信号
	emit VideoStart(fps);					//视频启动信号
	emit VideoTime(IO->totalMs);            //发送视频总时长

	while (isExit)
	{
		if (this->isPress)
		{
			QThread::msleep(5);
			continue;
		}
		while ( VidoeQueue.size() > 30 || AudioQueue.size() > 30 ) //AudioQueue.size() >20 ||
		{					
			QThread::msleep(5);
			continue;
		}
		int ret = IO->read(&pkt);  //读取视频
		if (ret != 0)
		{		
			QThread::msleep(1);
			continue;
		}
		//分离音视频		
		IO->AVSeparate(pkt);		
		QThread::msleep(8);
	}

	//avcodec_parameters_free(&as);
	//avcodec_parameters_free(&vs);
	ThreadClear();
	queueClear();		//队列内存清理
	close();
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

	//IO->ReadClear();  //清理读取缓冲
	IO->Seek(pos);
}


//清理线程缓冲
void Mt_ReadThread::ThreadClear()
{
	IO->ReadClear();
}

void Mt_ReadThread::queueClear()
{
	while (true)
	{
		if (AudioQueue.empty() && VidoeQueue.empty() && VidoeAVF.empty())
		{
			break;
		}

		if(!AudioQueue.empty())
		{
			AVPacket* pkt = AudioQueue.front();
			AudioQueue.pop();
			av_packet_free(&pkt);
			pkt = NULL;
		}
		if (!VidoeQueue.empty())
		{
			AVPacket* pkt = AudioQueue.front();
			AudioQueue.pop();
			av_packet_free(&pkt);
			pkt = NULL;
		}
		if (!VidoeAVF.empty())
		{
			AVFrame * frame = VidoeAVF.front();
			VidoeAVF.pop();
			av_frame_free(&frame);
			frame = NULL;
		}
	}

}