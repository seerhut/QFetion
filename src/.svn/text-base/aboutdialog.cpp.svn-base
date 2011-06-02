#include "aboutdialog.h"
#include <QDebug>
#include <QObject>

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent)
{
    m_ui.setupUi(this);
    qDebug() << "about created";
}

AboutDialog::~AboutDialog()
{
    qDebug() << "about destroyed";
}

void AboutDialog::changeEvent(QEvent *e)
{
    switch(e->type()) {
    case QEvent::LanguageChange:
        m_ui.retranslateUi(this);
        break;
    default:
        break;
    }
}
