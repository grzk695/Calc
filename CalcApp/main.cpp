#include "calcapp.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	CalcApp w;
	w.showMaximized();
	return a.exec();
}