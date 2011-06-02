#include "contactlistitem.h"
#include <QMouseEvent>

ContactListItem::ContactListItem(QWidget *parent) :
	QWidget(parent)
{
	m_ui.setupUi(this);
}

ContactListItem::~ContactListItem()
{

}

void ContactListItem::changeEvent(QEvent *e)
{
	switch (e->type())
	{
	case QEvent::LanguageChange:
		m_ui.retranslateUi(this);
		break;
	default:
		break;
	}
}

void ContactListItem::setNickName(const QString &str)
{
	m_ui.nicknameLB->setText(str);

}
void ContactListItem::setComplaint(const QString &str)
{

	m_ui.complaintLB->setText(str);
}

void ContactListItem::setPresence(const QString &str){
	m_ui.presenceText->setText(str);
}

void ContactListItem::setPresenceIcon(const QString &str){
	m_ui.presenceIcon->setPixmap(str);
}

void ContactListItem::mouseDoubleClickEvent ( QMouseEvent * event ){
	if(event->type() == QEvent::MouseButtonDblClick){
	doubleClicked();
	}
}
