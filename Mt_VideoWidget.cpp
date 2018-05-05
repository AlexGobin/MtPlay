#include "Mt_VideoWidget.h"


//�Զ���˫����
#define GET_STR(x) #x
#define A_VER 3
#define T_VER 4

//����shader
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

//ƬԪshader
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


//��ʼ���Կ�
void Mt_VideoWidget::Init(int width, int height, int videofmt)
{

	//std::cout << "--------------------�Կ�ģ���ʼ��!--------------------------" << std::endl;
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

	///��������ڴ�ռ�
	datas[0] = new unsigned char[this->Ysize];	//Y
	datas[1] = new unsigned char[this->Usize];	//U
	datas[2] = new unsigned char[this->Vsize];	//V


	if (texs[0])
	{
		glDeleteTextures(3, texs);
	}
	//��������
	glGenTextures(3, texs);

	//Y
	glBindTexture(GL_TEXTURE_2D, texs[0]);
	//�Ŵ���ˣ����Բ�ֵ   GL_NEAREST(Ч�ʸߣ�������������)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//���������Կ��ռ�
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, Ywidth, Yheight, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

	//U
	glBindTexture(GL_TEXTURE_2D, texs[1]);
	//�Ŵ���ˣ����Բ�ֵ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//���������Կ��ռ�
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, Uwidth , Uheight, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

	//V
	glBindTexture(GL_TEXTURE_2D, texs[2]);
	//�Ŵ���ˣ����Բ�ֵ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//���������Կ��ռ�
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, Vwidth, Vheight, 0, GL_RED, GL_UNSIGNED_BYTE, 0);
	mux.unlock();
}

//�Կ�ת�벢��ʾ
void Mt_VideoWidget::Repaint()
{
	//std::cout << "��Ƶ��ʾģ������!!!" << std::endl;

	if (vpts >apts)
	{
		return;
	}
	AVFrame *frame = VidoeAVF.pop();
	if (!frame)
	{
		return;
	}
		
	//�ݴ���֤�ߴ���ȷ
	if (!datas[0] || width*height == 0 || frame->width != this->width || frame->height != this->height)
	{
		av_frame_free(&frame);
		//mux.unlock();
		return;
	}
	if (width == frame->linesize[0]) //�������
	{
		memcpy(datas[0], frame->data[0], Ysize);
		memcpy(datas[1], frame->data[1], Usize);
		memcpy(datas[2], frame->data[2], Vsize);
	}
	else//�ж�������
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
	//mux.lock();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texs[0]); //0��󶨵�Y����
										   //�޸Ĳ�������(�����ڴ�����)
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, Ywidth, Yheight, GL_RED, GL_UNSIGNED_BYTE, datas[0]);
	//��shader uni��������
	glUniform1i(unis[0], 0);


	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, texs[1]); //1��󶨵�U����
	//�޸Ĳ�������(�����ڴ�����)
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, Uwidth , Uheight , GL_RED, GL_UNSIGNED_BYTE, datas[1]);
	//��shader uni��������
	glUniform1i(unis[1], 1);

	glActiveTexture(GL_TEXTURE0 + 2);
	glBindTexture(GL_TEXTURE_2D, texs[2]); //2��󶨵�V����
										   //�޸Ĳ�������(�����ڴ�����)
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, Vwidth ,Vheight , GL_RED, GL_UNSIGNED_BYTE, datas[2]);
	//��shader uni��������
	glUniform1i(unis[2], 2);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	//qDebug() << "paintGL";
	//mux.unlock();
}

void Mt_VideoWidget::initializeGL()
{
	//mux.lock();
	//��ʼ��opengl ��QOpenGLFunctions�̳У����� 
	initializeOpenGLFunctions();

	//program����shader�������ƬԪ���ű�
	//ƬԪ�����أ�
	qDebug() << program.addShaderFromSourceCode(QGLShader::Fragment, tString);
	//����shader
	qDebug() << program.addShaderFromSourceCode(QGLShader::Vertex, vString);

	//���ö�������ı���
	program.bindAttributeLocation("vertexIn", A_VER);

	//���ò�������
	program.bindAttributeLocation("textureIn", T_VER);

	//����shader
	//qDebug() << "program.link() = " << program.link();
	//qDebug() << "program.bind() = " << program.bind();
	program.link();
	program.bind();
	//���ݶ���Ͳ�������
	//����
	static const GLfloat ver[] = {
		-1.0f,-1.0f,
		1.0f,-1.0f,
		-1.0f, 1.0f,
		1.0f,1.0f
	};

	//����
	static const GLfloat tex[] = {
		0.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f
	};

	//����
	glVertexAttribPointer(A_VER, 2, GL_FLOAT, 0, 0, ver);
	glEnableVertexAttribArray(A_VER);

	//����
	glVertexAttribPointer(T_VER, 2, GL_FLOAT, 0, 0, tex);
	glEnableVertexAttribArray(T_VER);


	//��shader��ȡ����
	unis[0] = program.uniformLocation("tex_y");
	unis[1] = program.uniformLocation("tex_u");
	unis[2] = program.uniformLocation("tex_v");

	//mux.unlock();
}

void Mt_VideoWidget::resizeGL(int width, int height)
{

}
