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

	static bool isExit;   //�߳�ֹͣ����

	static bool isPause;   //��ͣ���ű���

	   //����pts

public:
	void run();   //��Ƶ�����̲߳��ź���

	void set(bool isexit);  // ֹͣ�߳�

	void ThreadPress(bool isPause);  //��ͣ����

	void ThreadClear();// ����������



	std::mutex mux;
private:
	
};

#endif // MT_AUDIOTHREAD_H
