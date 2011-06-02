#include "chatwindow.h"
#include "global.h"
#include <QKeyEvent>

ChatWindow::ChatWindow( QWidget *parent ) :
	QMainWindow(parent)
{
	m_ui.setupUi(this);
	isClient = false;
	richText = false;
	toClient = false;

	KeyPressEater *keyPressEater = new KeyPressEater(this);
	m_ui.inputTE->installEventFilter(keyPressEater);

	connect(keyPressEater, SIGNAL(enterPressed()), m_ui.sendBTN, SLOT(click()));

	qDebug() << "ChatWindow Created";
}

ChatWindow::~ChatWindow()
{
	qDebug() << "ChatWindow Destroyed";
}

void ChatWindow::changeEvent( QEvent *e )
{
	switch( e->type() )
	{
		case QEvent::LanguageChange:
			m_ui.retranslateUi(this);
			break;
		default:
			break;
	}
}

void ChatWindow::setContactSid( const QString &str )
{
	m_ui.contactSidLB->setText(str);
}
void ChatWindow::setContactMobno( const QString &str )
{
	m_ui.contactMobnoLB->setText("M:" + str);
}

void ChatWindow::setClient( bool isclient )
{
	isClient = isclient;
	if( isClient )
	{//leave to user
		setRichText(false);
		setSendtoClient(true);
		qDebug() << "cw :" << "set isClient true";
	} else
	{
		setRichText(false);
		setSendtoClient(false);
		qDebug() << "cw :" << "set isClient false";
	}
}

void ChatWindow::setSendtoClient( bool toclient )
{
	if( toclient && isClient )
	{
		toClient = true;
		m_ui.actionSend_to_Client->setChecked(true);
		m_ui.actionSend_to_Client->setText(tr("Send to Client"));
		m_ui.actionSend_to_Client->setToolTip(tr("Send to Client"));
		//		m_ui.actionSend_to_Client->setIcon(QIcon(":/client ena"));
		qDebug() << "cw :" << "isClient true, set Send_to_Client checked";
		return;

	}
	toClient = false;
	m_ui.actionSend_to_Client->setChecked(false);
	m_ui.actionSend_to_Client->setText(tr("Send to SMS"));
	m_ui.actionSend_to_Client->setToolTip(tr("Send to SMS"));
	//	m_ui.actionSend_to_Client->setIcon(QIcon(":/sms ena"));
	qDebug() << "cw :" << "isClient false, set Send_to_Client unchecked";

}

void ChatWindow::setRichText( bool rich )
{
	if( rich && isClient )
	{
		richText = true;
		m_ui.actionRich_Text->setChecked(true);
		m_ui.actionRich_Text->setText("Rich Text");
		//		m_ui.actionRich_Text->setIcon("");
		m_ui.actionRich_Text->setToolTip("Rich Text");
		qDebug() << "cw :" << "isClient true, set Rich_Text checked";
		return;

	}
	richText = false;
	m_ui.actionRich_Text->setChecked(false);
	m_ui.actionRich_Text->setText("Plain Text");
	//		m_ui.actionRich_Text->setIcon("");
	m_ui.actionRich_Text->setToolTip("Plain Text");
	qDebug() << "cw :" << "isClient true, set Rich_Text checked";
}

void ChatWindow::add_out_msg( const QString &name, const QString &str )
{//<p style=" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;
//	-qt-block-indent:0; text-indent:0px;"><span style=" color:#5555ff;">大熊</span>： 说话</p>
	this->m_ui.chatLogTB->append(
			tr("<p style=\" margin-top:0px; margin-bottom:0px; "
				"margin-left:0px; margin-right:0px;-qt-block-indent:0; text-indent:0px;\">"
				"<span style=\" color:#5555ff;\">%1</span>： %2</p>").arg(name).arg(
					str));
}

void ChatWindow::add_in_msg( const QString &name, const QString &str )
{//<p style=" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;
//	-qt-block-indent:0; text-indent:0px;"><span style=" color:#aa0000;">小熊</span>： 落落</p>
	this->m_ui.chatLogTB->append(
			tr("<p style=\" margin-top:0px; margin-bottom:0px; "
				"margin-left:0px; margin-right:0px;-qt-block-indent:0; text-indent:0px;\">"
				"<span style=\" color:#aa0000;\">%1</span>： %2</p>").arg(name).arg(
					str));
}

void ChatWindow::add_sys_msg( const QString &str )
{
#ifdef QFETION_DEBUG_OUTPUT
	//this->m_ui.chatLogTB->append("==== " + str + " ====");
#endif /* QFETION_DEBUG_OUTPUT */
}

void ChatWindow::on_sendBTN_clicked()
{
	qDebug() << "send clicked";
	if( m_ui.inputTE->toPlainText().isEmpty() )
	{
		this->m_ui.chatLogTB->append("==== don't send empty message ====");
		return;
	}
	if( toClient )
	{
		//to client
		if( richText )
		{
			send_text_client(m_ui.inputTE->toHtml());
		} else
		{
			send_text_client(m_ui.inputTE->toPlainText());
		}
	} else
	{
		//to sms
		send_text_sms(m_ui.inputTE->toPlainText());
	}
	m_ui.inputTE->clear();
}

void ChatWindow::on_actionClose_activated()
{
	this->hide();
}

void ChatWindow::on_actionRich_Text_triggered()
{
	if( richText )
	{
		setRichText(false);
	} else
	{
		setRichText(true);
	}
}

void ChatWindow::on_actionSend_to_Client_triggered()
{
	if( toClient )
	{
		setSendtoClient(false);
	} else
	{
		setSendtoClient(true);
	}
}

void ChatWindow::on_actionSend_activated()
{
	m_ui.sendBTN->click();
}

void ChatWindow::focusInEvent( QFocusEvent * event )
{
	m_ui.inputTE->setFocus();
}
void ChatWindow::hideEvent( QHideEvent * event )
{
	illhide();
}

void ChatWindow::closeEvent( QCloseEvent * event )
{
	event->ignore();
	this->hide();
}
void ChatWindow::showEvent( QShowEvent * event )
{
	illshow();
	m_ui.inputTE->setFocus();
	//	updateShow();
}

KeyPressEater::KeyPressEater( QObject * parent )
{

}

bool KeyPressEater::eventFilter( QObject *obj, QEvent *event )
{
	if( event->type() == QEvent::KeyPress )
	{
		QKeyEvent *keyEvent = static_cast<QKeyEvent *> (event);
		if( (keyEvent->key() == Qt::Key_Return) || (keyEvent->key()
				== Qt::Key_Enter) )
		{
			enterPressed();
			return true;
		}
//		qDebug("Ate key press %d", keyEvent->key());
		return QObject::eventFilter(obj, event);
	} else
	{
		// standard event processing
		return QObject::eventFilter(obj, event);
	}
}
