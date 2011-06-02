#ifndef CONSOLE_H
#define CONSOLE_H

#include <QWidget>
#include <QDebug>
#include "ui_console.h"

class Console: public QWidget {

Q_OBJECT
	Q_DISABLE_COPY(Console)

public:
	explicit Console(QWidget *parent);
	virtual ~Console();

protected:
	virtual void changeEvent(QEvent *e);

private:
	Ui::Console console_ui;

private slots:
	void appendLog(const QString &);
	void on_clearBTN_clicked();
};

#endif // CONSOLE_H
