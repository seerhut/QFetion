#include "console.h"
#include "global.h"
#include <QDebug>
Console::Console(QWidget *parent) :
	QWidget(parent)
{
//	qDebug() << "Console Class Loading... ";
	console_ui.setupUi(this);
	QObject::connect(console_ui.CloseBtn, SIGNAL(clicked()), this, SLOT(close()));

	//	qDebug() << parent->x() << parent->y();
	//	this->move(parent->x() + parent->width(),parent->y());
	this->move(0, 0);
	qDebug() << "Console Class Loaded ";
}

Console::~Console()
{
	qDebug() << "Console Class unLoaded";
}

void Console::changeEvent(QEvent *e)
{
	switch (e->type())
	{
	case QEvent::LanguageChange:
		console_ui.retranslateUi(this);
		break;
	default:
		break;
	}
}

void Console::on_clearBTN_clicked(){
	console_ui.LogTB->clear();
}

void Console::appendLog(const QString &string)
{
#ifdef QFETION_DEBUG_OUTPUT
	this->console_ui.LogTB->append(string);
	qDebug() << string;
#endif /* QFETION_DEBUG_OUTPUT */
}
