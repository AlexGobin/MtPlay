#include "Mt_deque.h"


long long apts;  //��ƵPTS
long long vpts;  //��ƵPTS


AVStream *avs = NULL;

std::queue<AVPacket*> AudioQueue;	//��Ƶpkt����
std::queue<AVPacket*> VidoeQueue;	//��Ƶpkt����

std::queue<AVFrame *> VidoeAVF;		//��ƵFrame����

AVCodecParameters *vs = NULL;
AVCodecParameters *as = NULL;
