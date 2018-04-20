#include "mtplay.h"
#include <QFileDialog>


static bool isOk = false;

mtplay::mtplay(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//启动所有线程
	openThread();

	//定时器
	timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, &mtplay::RefreshTimer);
	timer->start(40);

	//打开按钮
	connect(ui.btn_open, &QPushButton::clicked, this, &mtplay::openfile);
	
	//链接 videothread 和 
	connect(VideoThread, &Mt_VideoThread::show, ui.video, &Mt_VideoWidget::Repaint);

	connect(ReadThread, &Mt_ReadThread::Initshow, ui.video, &Mt_VideoWidget::Init);
	//emit videoTd("rtsp://admin:yjb695368576@[192.168.1.108]:554/cam/realmonitor?channel=1&subtype=0");

	connect(ReadThread, &Mt_ReadThread::VideoTime, this, &mtplay::ShowVideoTime);

	//滑动条事件
	connect(ui.mySlider, &QSlider::sliderPressed, this, &mtplay::SliderPress);
	connect(ui.mySlider, &QSlider::sliderReleased, this, &mtplay::SliderRelease);

	///右上角关闭链接槽函数
	connect(this, &mtplay::destroyed, this, &mtplay::myClose);

	///
	connect(ui.btn_play, &QPushButton::clicked, this, &mtplay::isPause);

}

mtplay::~mtplay()
{
	
	
}

//打开文件
void mtplay::openfile()
{
	//std::cout << "open file!" << std::endl;
	QString filename = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("选择视频文件"));
	if (filename.isEmpty())
	{
		return;
	}	
	emit readTd(filename);
}

//
void mtplay::isPause()
{
	isOk = !isOk;
	ThreadPress(isOk);
}

//显示视频总时长
void mtplay::ShowVideoTime(int totalMs1)
{
	this->totalSec = totalMs1;
	char buf[128] = { 0 };
	int min = (totalMs1 / 1000) / 60;
	int sec = (totalMs1 / 1000) % 60;
	sprintf(buf, "%03d:%02d", min, sec);
	ui.label_timeplay->setText(buf);
}



//鼠标按下滑动条事件
void mtplay::SliderPress()
{
	ThreadPress(true);
	timer->stop();
	ReadThread->queueClear();
	//ThreadClear();

}

//鼠标松开滑动条事件
void mtplay::SliderRelease()
{
	
	//std::cout << "松开了滑动条!" << std::endl;
	double pos = 0.0;
	pos = (double)ui.mySlider->value() / (double)ui.mySlider->maximum();
	std::cout << "pos:" << pos << std::endl;
	//ui.mySlider->setValue(pos * 1000);

	QThread::msleep(3);
	ReadThread->seek(pos); //清空缓冲
	
	ThreadPress(false);
	timer->start(40);
}


//右上角关闭链接槽函数
void mtplay::myClose()
{
	//回收视频线程
	delete timer;  //回收定时器
	VideoThread->set(false);
	AudiaoThread->set(false);
	ReadThread->set(false);


	QVideoThread->quit();
	QVideoThread->wait();
	delete QVideoThread;
	delete VideoThread;

	//回收音频线程

	QAudiaoThread->quit();
	QAudiaoThread->wait();
	delete QAudiaoThread;
	delete AudiaoThread;
	//回收音视频分离线程

	QReadThread->quit();
	QReadThread->wait();
	delete ReadThread;
	delete QReadThread;

}

//启动所有线程
void mtplay::openThread()
{
	//初始化音视频分离线程
	ReadThread = new Mt_ReadThread;
	QReadThread = new QThread(this);
	ReadThread->moveToThread(QReadThread);
	connect(this, &mtplay::readTd, ReadThread, &Mt_ReadThread::run);
	QReadThread->start();

	//初始化videoThread 线程
	VideoThread = new Mt_VideoThread;
	QVideoThread = new QThread(this);
	VideoThread->moveToThread(QVideoThread);
	connect(ReadThread, &Mt_ReadThread::VideoStart, VideoThread, &Mt_VideoThread::run);
	QVideoThread->start();	

	//初始化音频解码线程
	AudiaoThread = new Mt_AudioThread;
	QAudiaoThread = new QThread(this);
	AudiaoThread->moveToThread(QAudiaoThread);
	connect(ReadThread, &Mt_ReadThread::AudioStart, AudiaoThread, &Mt_AudioThread::run);
	QAudiaoThread->start();	
}



//暂停所有线程
void mtplay::ThreadPress(bool isPress)
{
	ReadThread->ThreadPress(isPress);
	VideoThread->ThreadPress(isPress);
	AudiaoThread->ThreadPress(isPress);
}

//清空所有线程缓冲
void mtplay::ThreadClear()
{
	ReadThread->ThreadClear();
	VideoThread->ThreadClear();
	AudiaoThread->ThreadClear();
}

void mtplay::ThreadClose()
{
	ReadThread->set(false);
	VideoThread->set(false);
	AudiaoThread->set(false);


}

void mtplay::resizeEvent(QResizeEvent *e)
{
	//ui.video->resize(size());
	//ui.btn_open->move(10, this->height() - 40);
	//ui.btn_play->move(this->width() - 60, this->height() - 40);
	//ui.mySlider->move(80, this->height() - 32);

}

void mtplay::videosize(int width, int height)
{
	//设置窗口大小
	//this->resize(QSize(width, height));
	//std::cout << "width:" << width << "height" << height << std::endl;
}

//重写定时器函数
void mtplay::RefreshTimer()
{
	char buf[32] = { 0 };
	int min = (apts / 1000) / 60;
	int sec = (apts / 1000) % 60;
	sprintf(buf, "%03d:%02d", min, sec);
	ui.label_time->setText(buf);
	if (totalSec >0)
	{
		float rate = (float)apts / (float)totalSec;
		ui.mySlider->setValue(rate * 1000);
	}

}
