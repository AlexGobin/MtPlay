#ifndef  __MT_VIDEODECODE_H__ 
#define	 __MT_VIDEODECODE_H__

#include "Mt_deque.h"


class Mt_VideoDecode
{
public:
	Mt_VideoDecode();
	~Mt_VideoDecode();

public:
	//视频解码器
	AVCodec *vcodec = NULL;
	//解码的PTS
	long long pts = 0;

public:
	//打开视频解码器
	bool open(AVCodecParameters *para);

	//发送到解码线程,不管成功还是失败都清理pkt
	bool send(AVPacket *pkt);

	//获取解码数据，一次send可能需要多次Recv，获取缓冲中的数据Send NULL在Recv多次
	//每次复制一份，由调用者释放 av_frame_free
	bool recv(AVFrame ** frame);

	//清理解码缓冲
	void DecodeClear();

	//清理回收内存.
	void close();
protected:
	AVCodecContext *codec = NULL;
	std::mutex mux;
};

#endif
