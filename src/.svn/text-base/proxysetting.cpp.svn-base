#include "proxysetting.h"
#include "ui_proxysetting.h"
#include "sysconfig.h"
#include <QNetworkProxy>

ProxySetting::ProxySetting( QWidget *parent ) :
	QDialog(parent), m_ui(new Ui::ProxySetting)
{
	m_ui->setupUi(this);
}

void ProxySetting::initFromSysconfig()
{
	m_ui->typeCB->setCurrentIndex(sysconfig->getPxType());
	m_ui->hostLE->setText(sysconfig->getPxHost());
	m_ui->portLE->setText(QString::number(sysconfig->getPxPort()));
	m_ui->passLE->setText(sysconfig->getPxPass());
	m_ui->userLE->setText(sysconfig->getPxUser());
	on_typeCB_currentIndexChanged(sysconfig->getPxType());
}

ProxySetting::~ProxySetting()
{
	delete m_ui;
}

void ProxySetting::on_typeCB_currentIndexChanged( int id )
{
	switch( id )
	{
		case 0:
			m_ui->hostLE->setDisabled(true);
			m_ui->portLE->setDisabled(true);
			m_ui->passLE->setDisabled(true);
			m_ui->userLE->setDisabled(true);
			break;
		case 1:
			m_ui->hostLE->setDisabled(false);
			m_ui->portLE->setDisabled(false);
			m_ui->passLE->setDisabled(true);
			m_ui->userLE->setDisabled(true);
			break;
		case 2:
			m_ui->hostLE->setDisabled(false);
			m_ui->portLE->setDisabled(false);
			m_ui->passLE->setDisabled(false);
			m_ui->userLE->setDisabled(false);
			break;
		default:
			;
	}
}

void ProxySetting::on_okBTN_clicked()
{

	QNetworkProxy proxy;
	switch( m_ui->typeCB->currentIndex() )
	{
		case 1:
			proxy.setType(QNetworkProxy::HttpProxy);
			break;
		case 2:
			proxy.setType(QNetworkProxy::Socks5Proxy);
			break;
		case 0:
		default:
			proxy.setType(QNetworkProxy::NoProxy);
	}
	proxy.setHostName(m_ui->hostLE->text());
	proxy.setPort(m_ui->portLE->text().toInt());
	if( m_ui->typeCB->currentIndex() == 2 )
	{
		proxy.setUser(m_ui->userLE->text());
		proxy.setPassword(m_ui->passLE->text());
	}else{
		proxy.setUser("");
		proxy.setPassword("");
	}
	QNetworkProxy::setApplicationProxy(proxy);
	this->hide();
	sysconfig->setPxInfo(m_ui->typeCB->currentIndex(), m_ui->hostLE->text(),
			m_ui->portLE->text().toInt(), m_ui->userLE->text(),
			m_ui->passLE->text());
}

void ProxySetting::on_cancelBTN_clicked()
{
	this->hide();
	this->initFromSysconfig();
}

void ProxySetting::showEvent( QShowEvent * event )
{
	initFromSysconfig();
}

void ProxySetting::changeEvent( QEvent *e )
{
	switch( e->type() )
	{
		case QEvent::LanguageChange:
			m_ui->retranslateUi(this);
			break;
		default:
			break;
	}
}
