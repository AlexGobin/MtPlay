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
	static bool isExit;  //����run�߳�ѭ��

	static bool isPress;  //�߳���ͣ

	int totalMS = 0; //��Ƶ��ʱ��

public:
	void run(QString filename);  //�̴߳�����

	void set(bool isexit);		//�߳�ֹͣ

	void ThreadPress(bool isPress);  //�߳���ͣ

	void close();  //�߳�������

	void seek(double pos);  //����λ�ú���

	void ThreadClear();    //�����̻߳���

	void queueClear();    //���pkt����
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
