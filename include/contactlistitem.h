#ifndef CONTACTLISTITEM_H
#define CONTACTLISTITEM_H

#include <QtGui/QWidget>
#include "ui_contactlistitem.h"

class QMouseEvent;

namespace Ui {
    class ContactListItem;
}

class ContactListItem : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(ContactListItem)
public:
    explicit ContactListItem(QWidget *parent = 0);
    virtual ~ContactListItem();
    void setNickName(const QString &);
    void setComplaint(const QString &);
    void setPresence(const QString &);
    void setPresenceIcon(const QString &);

signals:
	void doubleClicked();
protected:
    virtual void changeEvent(QEvent *e);
    void mouseDoubleClickEvent ( QMouseEvent * event );

private:
    Ui::ContactListItem m_ui;

};

#endif // CONTACTLISTITEM_H
