#include "Mt_deque.h"


long long apts;  //音频PTS
long long Apts;		//
long long vpts;  //视频PTS


AVStream *avs = NULL;

HFYqueue<AVPacket*> AudioQueue;
HFYqueue<AVPacket*> VideoQueue;
HFYqueue<AVFrame*> VidoeAVF;

// std::queue<AVPacket*> AudioQueue;	//音频pkt队列
// std::queue<AVPacket*> VidoeQueue;	//视频pkt队列
// 
// std::queue<AVFrame *> VidoeAVF;		//视频Frame队列

AVCodecParameters *vs = NULL;
AVCodecParameters *as = NULL;


