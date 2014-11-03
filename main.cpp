#include "scada.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Scada w;
	w.show();
	return a.exec();
}
