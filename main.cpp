#include "mtplay.h"
#include <QtWidgets/QApplication>

#include "Mt_OpenVideo.h"
#include "Mt_VideoDecode.h"
#include "Mt_deque.h"



int main(int argc, char *argv[])
{

	QApplication a(argc, argv);	
	mtplay w;
	w.show();
	return a.exec();
}
