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
	//初始化opengl
	void Init(int width, int height , int videofmt);
	//显卡转码
	void Repaint();
protected:
	//刷新显示
	void paintGL();
	//初始化gl
	void initializeGL();
	// 窗口尺寸变化
	void resizeGL(int width, int height);

private:
	int Ysize = 0;		//Y
	int Vsize = 0;		//U						
	int Usize = 0;		//V

	int Ywidth = 0;		//Y
	int Yheight = 0;

	int Uwidth = 0;		//U
	int Uheight = 0;

	int Vwidth = 0;		//V
	int Vheight = 0;


	std::mutex mux;
	//shader程序
	QGLShaderProgram program;

	//shader中yuv变量地址
	GLuint unis[3] = { 0 };
	//openg的 texture地址
	GLuint texs[3] = { 0 };

	//材质内存空间
	unsigned char *datas[3] = { 0 };

	int width = 640;
	int height = 380;
};

#endif // MT_VIDEOWIDGET_H
