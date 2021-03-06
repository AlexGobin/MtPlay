#include "Mt_VideoWidget.h"


//自动加双引号
#define GET_STR(x) #x
#define A_VER 3
#define T_VER 4

//顶点shader
const char *vString = GET_STR(
	attribute vec4 vertexIn;
	attribute vec2 textureIn;
	varying vec2 textureOut;
void main(void)
{
	gl_Position = vertexIn;
	textureOut = textureIn;
}
);

//片元shader
const char *tString = GET_STR(
	varying vec2 textureOut;
	uniform sampler2D tex_y;
	uniform sampler2D tex_u;
	uniform sampler2D tex_v;

void main(void)
{
	vec3 yuv;
	vec3 rgb;
	yuv.x = texture2D(tex_y, textureOut).r;
	yuv.y = texture2D(tex_u, textureOut).r - 0.5;
	yuv.z = texture2D(tex_v, textureOut).r - 0.5;
	rgb = mat3(1.0, 1.0, 1.0,
		0.0, -0.39465, 2.03211,
		1.13983, -0.58060, 0.0) * yuv;
	gl_FragColor = vec4(rgb, 1.0);
}
);


Mt_VideoWidget::Mt_VideoWidget(QWidget *parent)
	: QOpenGLWidget(parent)
{

}

Mt_VideoWidget::~Mt_VideoWidget()
{

}


//初始化显卡
void Mt_VideoWidget::Init(int width, int height, int videofmt)
{

	//std::cout << "--------------------显卡模块初始化!--------------------------" << std::endl;
	mux.lock();
	this->width = width;
	this->height = height;
	delete datas[0];
	delete datas[1];
	delete datas[2];

	//YUV420
	if (videofmt == 0)
	{
		std::cout << "---- YUV420 ----"<<std::endl;
		this->Ysize = width * height;
		this->Usize = width * height / 4;
		this->Vsize = width * height / 4;

		this->Ywidth = width;
		this->Yheight = height;

		this->Uwidth = width / 2;
		this->Uheight = height / 2;

		this->Vwidth = width / 2;
		this->Vheight = height / 2;
	}

	else if (videofmt == 4)
	{
		std::cout << "---- YUV422 ----" << std::endl;
		this->Ysize = width * height;
		this->Usize = width * height;
		this->Vsize = width * height;

		this->Ywidth = width;
		this->Yheight = height;

		this->Uwidth = width;
		this->Uheight = height/2;

		this->Vwidth = width;
		this->Vheight = height/2;
	}

	//YUV444
	else if (videofmt == 5)
	{
		std::cout << "---- YUV444 ----" << std::endl;
		this->Ysize = width * height;
		this->Usize = width * height;
		this->Vsize = width * height;

		this->Ywidth = width;
		this->Yheight = height;

		this->Uwidth = width ;
		this->Uheight = height;

		this->Vwidth = width ;
		this->Vheight = height ;
	}

	///分配材质内存空间
	datas[0] = new unsigned char[this->Ysize];	//Y
	datas[1] = new unsigned char[this->Usize];	//U
	datas[2] = new unsigned char[this->Vsize];	//V


	if (texs[0])
	{
		glDeleteTextures(3, texs);
	}
	//创建材质
	glGenTextures(3, texs);

	//Y
	glBindTexture(GL_TEXTURE_2D, texs[0]);
	//放大过滤，线性插值   GL_NEAREST(效率高，但马赛克严重)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//创建材质显卡空间
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, Ywidth, Yheight, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

	//U
	glBindTexture(GL_TEXTURE_2D, texs[1]);
	//放大过滤，线性插值
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//创建材质显卡空间
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, Uwidth , Uheight, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

	//V
	glBindTexture(GL_TEXTURE_2D, texs[2]);
	//放大过滤，线性插值
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//创建材质显卡空间
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, Vwidth, Vheight, 0, GL_RED, GL_UNSIGNED_BYTE, 0);
	mux.unlock();
}

//显卡转码并显示
void Mt_VideoWidget::Repaint()
{
	//std::cout << "视频显示模块启动!!!" << std::endl;

	AVFrame *frame = VidoeAVF.pop();
	if (!frame)
	{
		return;
	}
		
	//容错，保证尺寸正确
	if (!datas[0] || width*height == 0 || frame->width != this->width || frame->height != this->height)
	{
		av_frame_free(&frame);
		//mux.unlock();
		return;
	}
	if (width == frame->linesize[0]) //无需对齐
	{
		memcpy(datas[0], frame->data[0], Ysize);
		memcpy(datas[1], frame->data[1], Usize);
		memcpy(datas[2], frame->data[2], Vsize);
	}
	else//行对齐问题
	{
		//Y 
		for (int i = 0; i < height; i++)
		{
			memcpy(datas[0] + Ywidth * i, frame->data[0] + frame->linesize[0] * i, width);
		}
			
		//U
		for (int i = 0; i < height / 2; i++)
		{
			memcpy(datas[1] + Uwidth  * i, frame->data[1] + frame->linesize[1] * i, width);
		}
			
		//V
		for (int i = 0; i < height / 2; i++)
		{
			memcpy(datas[2] + Vwidth  * i, frame->data[2] + frame->linesize[2] * i, width);
		}			
	}

	//mux.unlock();
	av_frame_free(&frame);
	update();
}

void Mt_VideoWidget::paintGL()
{
	mux.lock();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texs[0]); //0层绑定到Y材质
										   //修改材质内容(复制内存内容)
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, Ywidth, Yheight, GL_RED, GL_UNSIGNED_BYTE, datas[0]);
	//与shader uni遍历关联
	glUniform1i(unis[0], 0);


	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, texs[1]); //1层绑定到U材质
	//修改材质内容(复制内存内容)
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, Uwidth , Uheight , GL_RED, GL_UNSIGNED_BYTE, datas[1]);
	//与shader uni遍历关联
	glUniform1i(unis[1], 1);

	glActiveTexture(GL_TEXTURE0 + 2);
	glBindTexture(GL_TEXTURE_2D, texs[2]); //2层绑定到V材质
										   //修改材质内容(复制内存内容)
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, Vwidth ,Vheight , GL_RED, GL_UNSIGNED_BYTE, datas[2]);
	//与shader uni遍历关联
	glUniform1i(unis[2], 2);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	//qDebug() << "paintGL";
	mux.unlock();
}

void Mt_VideoWidget::initializeGL()
{
	mux.lock();
	//初始化opengl （QOpenGLFunctions继承）函数 
	initializeOpenGLFunctions();

	//program加载shader（顶点和片元）脚本
	//片元（像素）
	qDebug() << program.addShaderFromSourceCode(QGLShader::Fragment, tString);
	//顶点shader
	qDebug() << program.addShaderFromSourceCode(QGLShader::Vertex, vString);

	//设置顶点坐标的变量
	program.bindAttributeLocation("vertexIn", A_VER);

	//设置材质坐标
	program.bindAttributeLocation("textureIn", T_VER);

	//编译shader
	//qDebug() << "program.link() = " << program.link();
	//qDebug() << "program.bind() = " << program.bind();
	program.link();
	program.bind();
	//传递顶点和材质坐标
	//顶点
	static const GLfloat ver[] = {
		-1.0f,-1.0f,
		1.0f,-1.0f,
		-1.0f, 1.0f,
		1.0f,1.0f
	};

	//材质
	static const GLfloat tex[] = {
		0.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f
	};

	//顶点
	glVertexAttribPointer(A_VER, 2, GL_FLOAT, 0, 0, ver);
	glEnableVertexAttribArray(A_VER);

	//材质
	glVertexAttribPointer(T_VER, 2, GL_FLOAT, 0, 0, tex);
	glEnableVertexAttribArray(T_VER);


	//从shader获取材质
	unis[0] = program.uniformLocation("tex_y");
	unis[1] = program.uniformLocation("tex_u");
	unis[2] = program.uniformLocation("tex_v");

	mux.unlock();
}

void Mt_VideoWidget::resizeGL(int width, int height)
{

}
