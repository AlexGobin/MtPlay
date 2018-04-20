#include "mtplay.h"
#include <QFileDialog>


static bool isOk = false;

mtplay::mtplay(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//���������߳�
	openThread();

	//��ʱ��
	timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, &mtplay::RefreshTimer);
	timer->start(40);

	//�򿪰�ť
	connect(ui.btn_open, &QPushButton::clicked, this, &mtplay::openfile);
	
	//���� videothread �� 
	connect(VideoThread, &Mt_VideoThread::show, ui.video, &Mt_VideoWidget::Repaint);

	connect(ReadThread, &Mt_ReadThread::Initshow, ui.video, &Mt_VideoWidget::Init);
	//emit videoTd("rtsp://admin:yjb695368576@[192.168.1.108]:554/cam/realmonitor?channel=1&subtype=0");

	connect(ReadThread, &Mt_ReadThread::VideoTime, this, &mtplay::ShowVideoTime);

	//�������¼�
	connect(ui.mySlider, &QSlider::sliderPressed, this, &mtplay::SliderPress);
	connect(ui.mySlider, &QSlider::sliderReleased, this, &mtplay::SliderRelease);

	///���Ͻǹر����Ӳۺ���
	connect(this, &mtplay::destroyed, this, &mtplay::myClose);

	///
	connect(ui.btn_play, &QPushButton::clicked, this, &mtplay::isPause);

}

mtplay::~mtplay()
{
	
	
}

//���ļ�
void mtplay::openfile()
{
	//std::cout << "open file!" << std::endl;
	QString filename = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("ѡ����Ƶ�ļ�"));
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

//��ʾ��Ƶ��ʱ��
void mtplay::ShowVideoTime(int totalMs1)
{
	this->totalSec = totalMs1;
	char buf[128] = { 0 };
	int min = (totalMs1 / 1000) / 60;
	int sec = (totalMs1 / 1000) % 60;
	sprintf(buf, "%03d:%02d", min, sec);
	ui.label_timeplay->setText(buf);
}



//��갴�»������¼�
void mtplay::SliderPress()
{
	ThreadPress(true);
	timer->stop();
	ReadThread->queueClear();
	//ThreadClear();

}

//����ɿ��������¼�
void mtplay::SliderRelease()
{
	
	//std::cout << "�ɿ��˻�����!" << std::endl;
	double pos = 0.0;
	pos = (double)ui.mySlider->value() / (double)ui.mySlider->maximum();
	std::cout << "pos:" << pos << std::endl;
	//ui.mySlider->setValue(pos * 1000);

	QThread::msleep(3);
	ReadThread->seek(pos); //��ջ���
	
	ThreadPress(false);
	timer->start(40);
}


//���Ͻǹر����Ӳۺ���
void mtplay::myClose()
{
	//������Ƶ�߳�
	delete timer;  //���ն�ʱ��
	VideoThread->set(false);
	AudiaoThread->set(false);
	ReadThread->set(false);


	QVideoThread->quit();
	QVideoThread->wait();
	delete QVideoThread;
	delete VideoThread;

	//������Ƶ�߳�

	QAudiaoThread->quit();
	QAudiaoThread->wait();
	delete QAudiaoThread;
	delete AudiaoThread;
	//��������Ƶ�����߳�

	QReadThread->quit();
	QReadThread->wait();
	delete ReadThread;
	delete QReadThread;

}

//���������߳�
void mtplay::openThread()
{
	//��ʼ������Ƶ�����߳�
	ReadThread = new Mt_ReadThread;
	QReadThread = new QThread(this);
	ReadThread->moveToThread(QReadThread);
	connect(this, &mtplay::readTd, ReadThread, &Mt_ReadThread::run);
	QReadThread->start();

	//��ʼ��videoThread �߳�
	VideoThread = new Mt_VideoThread;
	QVideoThread = new QThread(this);
	VideoThread->moveToThread(QVideoThread);
	connect(ReadThread, &Mt_ReadThread::VideoStart, VideoThread, &Mt_VideoThread::run);
	QVideoThread->start();	

	//��ʼ����Ƶ�����߳�
	AudiaoThread = new Mt_AudioThread;
	QAudiaoThread = new QThread(this);
	AudiaoThread->moveToThread(QAudiaoThread);
	connect(ReadThread, &Mt_ReadThread::AudioStart, AudiaoThread, &Mt_AudioThread::run);
	QAudiaoThread->start();	
}



//��ͣ�����߳�
void mtplay::ThreadPress(bool isPress)
{
	ReadThread->ThreadPress(isPress);
	VideoThread->ThreadPress(isPress);
	AudiaoThread->ThreadPress(isPress);
}

//��������̻߳���
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
	//���ô��ڴ�С
	//this->resize(QSize(width, height));
	//std::cout << "width:" << width << "height" << height << std::endl;
}

//��д��ʱ������
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
