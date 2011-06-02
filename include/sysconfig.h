#ifndef SYSCONFIG_H
#define SYSCONFIG_H

#include <QObject>
#include <QSettings>

class SysConfig : public QObject
{
Q_OBJECT

public:
	SysConfig( QObject * parent );
	void init();
	void initFromConfig();
	void updateFromInfoServer();
	void saveToConfig();

	const QString &getSsiPortalHost();
	const int getSsiPortalPort();
	const QString &getSsiPortalPath();//include "%1 %2"from mob_no and passwd
	const QString getDefaultAcount();
	const QString &getHelloStr();

	void setPxInfo(int type, QString host, int port, QString user, QString pass);
	int getPxType(){ return px_type;};
	QString& getPxHost(){ return px_host;};
	int getPxPort(){ return px_port;};
	QString& getPxUser(){ return px_user;};
	QString& getPxPass(){ return px_pass;};

//	bool hasDefaultAcount();
//	bool hasConfigFor( const QString & );

//	void setSsiPortalHost( const QString & );
//	void setSsiPortalPort( int );
//	void setSsiPortalPath( const QString & );
//	void setHelloStr( const QString & );

	signals:
	void addtoLog( const QString & );

private:
	QString ssihost, ssipath,  hello_str, px_host, px_user, px_pass;
	int ssiport, px_port, px_type;
	QSettings settings;
};

#endif // SYSCONFIG_H
