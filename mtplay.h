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
	//�򿪰�ť�ۺ���
	void openfile();

	//��ͣ����
	void isPause();

	//��ʾ��Ƶ��ʱ��
	void ShowVideoTime(int totalMs1);

	//��갴�»������¼�
	void SliderPress();

	//����ɿ��������¼�
	void SliderRelease();

	//���Ͻǹر����Ӳۺ���
	void myClose();

public:
	//��Ƶ�����̺߳���
	Mt_VideoThread * VideoThread = NULL;
	QThread * QVideoThread = NULL;

	//��Ƶ����
	Mt_AudioThread * AudiaoThread = NULL;
	QThread * QAudiaoThread = NULL;

	//����Ƶ�����߳�
	Mt_ReadThread * ReadThread = NULL;
	QThread * QReadThread = NULL;

	int totalSec = 0;

public:
	// �������߳�
	void openThread(); 

	//��ͣ�����߳�
	void ThreadPress(bool isPress);

	//��������̻߳���
	void ThreadClear();

	//�˳������̺߳���ѭ��
	void ThreadClose();

	 //���ڳߴ�仯
	void resizeEvent(QResizeEvent *e);

	//������Ƶ���ô��ڴ�С
	void videosize(int width, int height);

	//��д��ʱ������
	void RefreshTimer();

	
private:
	Ui::mtplayClass ui;

	bool isPress = false;

	QTimer * timer = NULL;  //��ʱ��

signals:
	void videoTd();						//��Ƶ�����߳��ź�

	void audioTd();						//��Ƶ�����߳��ź�

	void readTd(QString filename);		//����Ƶ�����߳������ź�
};

#endif // MTPLAY_H
