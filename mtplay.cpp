#include "mtplay.h"
#include <QFileDialog>


static bool isOk = false;

#define VIDEO_PAUSE_1  "QPushButton{border-image: url(:/im/images/pause.png);}"
#define VIDEO_PLAY_1   "QPushButton{border-image: url(:/im/images/play.png);}"


mtplay::mtplay(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	this->resize(640, 480);

	
	//打开按钮
	connect(ui.btn_open, &QPushButton::clicked, this, &mtplay::openfile);
	connect(ui.btn_stop, &QPushButton::clicked, this, &mtplay::playClose);
	
	
	//滑动条事件
	connect(ui.mySlider, &QSlider::sliderPressed, this, &mtplay::SliderPress);
	connect(ui.mySlider, &QSlider::sliderReleased, this, &mtplay::SliderRelease);

	///右上角关闭链接槽函数
	connect(this, &mtplay::destroyed, this, &mtplay::myClose);
	connect(ui.btn_play, &QPushButton::clicked, this, &mtplay::isPause);

}

mtplay::~mtplay()
{
	
}

//打开文件
void mtplay::openfile()
{
	//std::cout << "open file!" << std::endl;
	QString filename = QFileDialog::getOpenFileName(this,"open","../",
		"MP4(*.mp4 *.avi *.rmvb *.flv *.mkv *.)");
	if (filename.isEmpty())
	{
		return;
	}	
	//启动所有线程
	openThread();
	VideoThread->set(true);
	AudiaoThread->set(true);
	ReadThread->set(true);

	this->setWindowTitle(filename);//设置窗口标题
	ui.btn_play->setStyleSheet(VIDEO_PAUSE_1);
	emit readTd(filename);	

	//发动条移动定时器
	timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, &mtplay::RefreshTimer);
	timer->start(40);
}

//
void mtplay::isPause()
{
	isOk = !isOk;
	if (isOk == false)
	{
		ui.btn_play->setStyleSheet(VIDEO_PAUSE_1);
	}
	else
	{
		ui.btn_play->setStyleSheet(VIDEO_PLAY_1);
	}
	
	ThreadPress(isOk);
}


void mtplay::playClose()
{
	if (QReadThread != NULL || QVideoThread != NULL || QAudiaoThread != NULL)
	{
		
		VideoThread->set(false);
		AudiaoThread->set(false);
		ReadThread->set(false);
	
	}
	if (timer != NULL)
	{
		timer->stop();
		delete timer;
		timer = NULL;
	}
}

//显示视频总时长
void mtplay::ShowVideoTime(int totalMs1)
{
	this->totalSec = totalMs1;
	char buf[128] = { 0 };
	int min = (totalMs1 / 1000) / 60;
	int sec = (totalMs1 / 1000) % 60;
	sprintf(buf, "%03d:%02d", min, sec);
	ui.total_time->setText(buf);
}



//鼠标按下滑动条事件
void mtplay::SliderPress()
{
	timer->stop();
	ThreadPress(true);	
}

//鼠标松开滑动条事件
void mtplay::SliderRelease()
{
	//std::cout << "松开了滑动条!" << std::endl;
	double pos = 0.0;
	pos = (double)ui.mySlider->value() / (double)ui.mySlider->maximum();
	std::cout << "pos:" << pos << std::endl;
	
	ReadThread->seek(pos); //清空缓冲	
	ThreadPress(false);
	timer->start(40);
}


//右上角关闭链接槽函数
void mtplay::myClose()
{

	this->playClose();

	if (QReadThread != NULL || QVideoThread != NULL || QAudiaoThread != NULL)
	{
		
		QVideoThread->quit();
		QVideoThread->wait();

		//回收音频线程
		QAudiaoThread->quit();
		QAudiaoThread->wait();

		//回收音视频分离线程
		QReadThread->quit();
		QReadThread->wait();

		delete AudiaoThread;
		delete VideoThread;
		delete ReadThread;
		AudiaoThread = NULL;
		VideoThread = NULL;
		ReadThread = NULL;

		delete QReadThread;
		delete QVideoThread;
		delete QAudiaoThread;
		QReadThread = NULL;
		QVideoThread = NULL;
		QAudiaoThread = NULL;
	}
}

//启动所有线程
void mtplay::openThread()
{
	if (QReadThread == NULL || QVideoThread ==NULL || QAudiaoThread == NULL)
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

		//链接 videothread 和 
		connect(VideoThread, &Mt_VideoThread::show, ui.video, &Mt_VideoWidget::Repaint);
		connect(ReadThread, &Mt_ReadThread::Initshow, ui.video, &Mt_VideoWidget::Init);
		connect(ReadThread, &Mt_ReadThread::VideoTime, this, &mtplay::ShowVideoTime);
	}
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

//退出所有线程循环
void mtplay::ThreadClose()
{
	ReadThread->set(false);
	VideoThread->set(false);
	AudiaoThread->set(false);
}

void mtplay::resizeEvent(QResizeEvent *e)
{
	
	ui.video->resize(this->width(), this->height() - 40);

	ui.btn_set->move(this->width() - 40, this->height() - 35);
	ui.btn_open->move(this->width()-85, this->height() - 35);
	ui.btn_play->move(this->width() - 130, this->height() - 35);
	ui.btn_stop->move(this->width() - 175, this->height() - 35);

	ui.play_time->move(20, this->height()-30);
	ui.total_time->move(this->width() - 245, this->height() - 30);


	ui.mySlider->move(80, this->height() - 30);
	ui.mySlider->resize(this->width() - 350, 20);

}

//双击全屏
void mtplay::mouseDoubleClickEvent(QMouseEvent *e)
{
	if (isFullScreen())
	{
		this->showNormal();
	}
	else
	{
		this->showFullScreen();
	}
}


//关闭视频播放
void mtplay::VideoStop()
{
	ThreadPress(true);
	//ThreadClear();
	ThreadClose();
}

//重写定时器函数
void mtplay::RefreshTimer()
{
	char buf[32] = { 0 };
	int min = (apts / 1000) / 60;
	int sec = (apts / 1000) % 60;
	sprintf(buf, "%03d:%02d", min, sec);
	ui.play_time->setText(buf);
	if (totalSec >0)
	{
		float rate = (float)apts / (float)totalSec;
		ui.mySlider->setValue(rate * 1000);
	}

}
