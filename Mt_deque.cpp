#include "Mt_deque.h"


long long apts;  //��ƵPTS
long long Apts;		//
long long vpts;  //��ƵPTS


AVStream *avs = NULL;

HFYqueue<AVPacket*> AudioQueue;
HFYqueue<AVPacket*> VideoQueue;
HFYqueue<AVFrame*> VidoeAVF;

// std::queue<AVPacket*> AudioQueue;	//��Ƶpkt����
// std::queue<AVPacket*> VidoeQueue;	//��Ƶpkt����
// 
// std::queue<AVFrame *> VidoeAVF;		//��ƵFrame����

AVCodecParameters *vs = NULL;
AVCodecParameters *as = NULL;


