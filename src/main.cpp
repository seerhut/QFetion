#include <QApplication>
#include <QTextCodec>
#include <QPlastiqueStyle>
#include <QTranslator>
#include "mainwindow.h"

int main( int argc, char *argv[] )
{
	QApplication app(argc, argv);
	QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
	QCoreApplication::setOrganizationName("Seerhut Labs");
	QCoreApplication::setOrganizationDomain("seerhut.com");
	QCoreApplication::setApplicationName("Qfetion");
#ifdef WIN32
	QApplication::setStyle(new QPlastiqueStyle);
#endif
	QTranslator qtTranslator;
	qtTranslator.load("qfetion_" + QLocale::system().name());
	qDebug() << "qfetion_" + QLocale::system().name();
	app.installTranslator(&qtTranslator);

	MainWindow w;
	w.show();
	return app.exec();
}
