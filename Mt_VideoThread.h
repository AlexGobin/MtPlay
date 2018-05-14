#ifndef MT_VIDEOTHREAD_H
#define MT_VIDEOTHREAD_H

#include <QObject>
#include "Mt_OpenVideo.h"
#include "Mt_VideoDecode.h"


class Mt_VideoThread : public QObject
{
	Q_OBJECT

public:
	explicit Mt_VideoThread(QObject *parent = nullptr);
	~Mt_VideoThread();
	
public:
	static bool isExit;  //线程停止变量

	static bool isPress;   //线程暂停

	Mt_VideoDecode * DE = NULL; //解码

	std::mutex mux;
public:
	// 线程函数
	void run(int fps);

	//设置线程循环停止变量
	void set(bool isexit);

	//线程暂停
	void ThreadPress(bool isPress);

	//线程缓存清理
	void ThreadClear();


signals:
	void show();
};

#endif // MT_VIDEOTHREAD_H
