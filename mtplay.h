#ifndef MTPLAY_H
#define MTPLAY_H

#include <QtWidgets/QWidget>
#include "ui_mtplay.h"
#include "Mt_VideoThread.h"
#include "Mt_deque.h"
#include "Mt_AudioThread.h"
#include "Mt_ReadThread.h"
#include <QTimer>

class mtplay : public QWidget
{
	Q_OBJECT

public:
	mtplay(QWidget *parent = 0);
	~mtplay();

public slots:
	//打开按钮槽函数
	void openfile();

	//暂停播放
	void isPause();

	//显示视频总时长
	void ShowVideoTime(int totalMs1);

	//鼠标按下滑动条事件
	void SliderPress();

	//鼠标松开滑动条事件
	void SliderRelease();

	//右上角关闭链接槽函数
	void myClose();

public:
	//视频解码线程函数
	Mt_VideoThread * VideoThread = NULL;
	QThread * QVideoThread = NULL;

	//音频解码
	Mt_AudioThread * AudiaoThread = NULL;
	QThread * QAudiaoThread = NULL;

	//音视频分离线程
	Mt_ReadThread * ReadThread = NULL;
	QThread * QReadThread = NULL;

	int totalSec = 0;

public:
	// 打开所有线程
	void openThread(); 

	//暂停所有线程
	void ThreadPress(bool isPress);

	//清空所有线程缓冲
	void ThreadClear();

	//退出所有线程函数循环
	void ThreadClose();

	 //窗口尺寸变化
	void resizeEvent(QResizeEvent *e);

	//根据视频设置窗口大小
	void videosize(int width, int height);

	//重写定时器函数
	void RefreshTimer();

	
private:
	Ui::mtplayClass ui;

	bool isPress = false;

	QTimer * timer = NULL;  //定时器

signals:
	void videoTd();						//视频启动线程信号

	void audioTd();						//音频启动线程信号

	void readTd(QString filename);		//音视频分离线程启动信号
};

#endif // MTPLAY_H
