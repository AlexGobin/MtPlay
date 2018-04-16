#ifndef MT_AUDIOTHREAD_H
#define MT_AUDIOTHREAD_H

#include "Mt_deque.h"
#include "Mt_AudioPlay.h"
#include <QObject>

class Mt_AudioThread : public QObject
{
	Q_OBJECT

public:
	explicit Mt_AudioThread(QObject *parent = nullptr);
	~Mt_AudioThread();

public:
	Mt_AudioPlay * audio = NULL;

	static bool isExit;   //线程停止变量

	static bool isPause;   //暂停播放变量

	   //计算pts

public:
	void run();   //音频解码线程播放函数

	void set(bool isexit);  // 停止线程

	void ThreadPress(bool isPause);  //暂停播放

	void ThreadClear();// 缓冲清理函数



	std::mutex mux;
private:
	
};

#endif // MT_AUDIOTHREAD_H
