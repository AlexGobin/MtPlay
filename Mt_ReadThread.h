#ifndef MT_READTHREAD_H
#define MT_READTHREAD_H

#include <QObject>
#include "Mt_OpenVideo.h"

#include "Mt_deque.h"

class Mt_ReadThread : public QObject
{
	Q_OBJECT

public:
	Mt_ReadThread(QObject *parent = NULL);
	~Mt_ReadThread();

public:
	static bool isExit;  //结束run线程循环

	static bool isPress;  //线程暂停

	int totalMS = 0; //视频总时长

public:
	void run(QString filename);  //线程处理函数

	void set(bool isexit);		//线程停止

	void ThreadPress(bool isPress);  //线程暂停

	void close();  //线程清理函数

	void seek(double pos);  //播放位置函数

	void ThreadClear();    //清理线程缓冲

	void queueClear();    //清空pkt队列
public:
	Mt_OpenVideo * IO = NULL;
	
	std::mutex mux;
signals:
	void VideoStart(int fps);

	void AudioStart();

	void Initshow(int w, int h);

	void VideoTime(int);
};

#endif // MT_READTHREAD_H
