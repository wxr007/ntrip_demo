#include "ntrip_qt.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ntrip_qt w;
	w.show();
	return a.exec();
}
