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
	static bool isExit;  //�߳�ֹͣ����

	static bool isPress;   //�߳���ͣ

	Mt_VideoDecode * DE = NULL; //����

	std::mutex mux;
public:
	// �̺߳���
	void run(int fps);

	//�����߳�ѭ��ֹͣ����
	void set(bool isexit);

	//�߳���ͣ
	void ThreadPress(bool isPress);

	//�̻߳�������
	void ThreadClear();


signals:
	void show();
};

#endif // MT_VIDEOTHREAD_H
