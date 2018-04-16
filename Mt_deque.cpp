#include "Mt_deque.h"


long long apts;  //音频PTS
long long vpts;  //视频PTS


AVStream *avs = NULL;

std::queue<AVPacket*> AudioQueue;	//音频pkt队列
std::queue<AVPacket*> VidoeQueue;	//视频pkt队列

std::queue<AVFrame *> VidoeAVF;		//视频Frame队列

AVCodecParameters *vs = NULL;
AVCodecParameters *as = NULL;
