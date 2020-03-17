#include "MyWiget.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MyWiget w;
	w.show();
	return a.exec();
}
