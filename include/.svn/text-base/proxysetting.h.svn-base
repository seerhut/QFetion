#ifndef PROXYSETTING_H
#define PROXYSETTING_H

#include <QtGui/QDialog>

namespace Ui
{
class ProxySetting;
}

class SysConfig;

class ProxySetting : public QDialog
{
Q_OBJECT
Q_DISABLE_COPY(ProxySetting)
public:
	explicit ProxySetting( QWidget *parent = 0 );
	virtual ~ProxySetting();
	void initFromSysconfig();
	void setSysconfig( SysConfig * sc )	{	sysconfig = sc;	};

protected:
	virtual void changeEvent( QEvent *e );
	virtual void showEvent( QShowEvent * event );

private slots:
	void on_typeCB_currentIndexChanged(int id);
	void on_okBTN_clicked();
	void on_cancelBTN_clicked();

private:
	Ui::ProxySetting *m_ui;
	SysConfig * sysconfig;
};

#endif // PROXYSETTING_H
