#ifndef MT_VIDEOWIDGET_H
#define MT_VIDEOWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QGLShaderProgram>
#include "Mt_deque.h"



class Mt_VideoWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT

public:
	Mt_VideoWidget(QWidget *parent);
	~Mt_VideoWidget();

public:
	//��ʼ��opengl
	void Init(int width, int height);
	//�Կ�ת��
	void Repaint();
protected:
	//ˢ����ʾ
	void paintGL();
	//��ʼ��gl
	void initializeGL();
	// ���ڳߴ�仯
	void resizeGL(int width, int height);

private:
	std::mutex mux;

	//shader����
	QGLShaderProgram program;

	//shader��yuv������ַ
	GLuint unis[3] = { 0 };
	//openg�� texture��ַ
	GLuint texs[3] = { 0 };

	//�����ڴ�ռ�
	unsigned char *datas[3] = { 0 };

	int width = 640;
	int height = 380;
};

#endif // MT_VIDEOWIDGET_H
