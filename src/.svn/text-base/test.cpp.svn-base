#include "test.h"
#include "ui_test.h"

Test::Test(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::Test)
{
    m_ui->setupUi(this);
}

Test::~Test()
{
    delete m_ui;
}

void Test::changeEvent(QEvent *e)
{
    switch(e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
