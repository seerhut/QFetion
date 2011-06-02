#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QtGui/QMainWindow>
#include "ui_chatwindow.h"
#include <QString>
#include <QDebug>
#include "control.h"

class ChatWindow : public QMainWindow
{
Q_OBJECT
	Q_DISABLE_COPY(ChatWindow)

public:
	enum presenceType
	{
		client, sms
	};
	explicit ChatWindow( QWidget *parent = 0 );
	virtual ~ChatWindow();
	//    void setControl(Control * control);
	void setContactSid( const QString & );
	void setContactMobno( const QString & );
	void setClient( bool isclient );
	void setSendtoClient( bool toclient );
	void setRichText( bool rich );
	//	void setSwitchSendToClientDisabled(bool);

signals:
	void send_msg( const QString &to_uri, const QString &msg );

public slots:
	void add_in_msg( const QString &, const QString & );
	void add_out_msg( const QString &, const QString & );
	void add_sys_msg( const QString & );
	void on_sendBTN_clicked();
	void on_actionRich_Text_triggered();
	void on_actionSend_to_Client_triggered();
	void on_actionClose_activated();
	void on_actionSend_activated();

signals:
	void updateShow();
	void send_text_client( const QString & );
	void send_text_sms( const QString & );
	void illshow();
	void illhide();

protected:
	virtual void changeEvent( QEvent *e );
	virtual void focusInEvent( QFocusEvent * event );
	virtual void hideEvent( QHideEvent * event );
	virtual void closeEvent( QCloseEvent * event );
	virtual void showEvent( QShowEvent * event );

private:
	Ui::ChatWindow m_ui;
	bool toClient;
	bool richText;
	bool isClient;
};

class KeyPressEater : public QObject
{
Q_OBJECT

public:
	KeyPressEater(QObject *parent);

	signals:
	void enterPressed();

protected:
	bool eventFilter( QObject *obj, QEvent *event );
};

#endif // CHATWINDOW_H
