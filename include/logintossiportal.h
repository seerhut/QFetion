#ifndef LOGINTOSSIPORTAL_H
#define LOGINTOSSIPORTAL_H

#include <QObject>
#include <QHttp>
#include <QSslSocket>
#include <QBuffer>
#include <QtXml>
#include <sysconfig.h>

class LoginToSsiPortal : public QHttp
{
Q_OBJECT

public:

	LoginToSsiPortal( QObject * parent = 0 );
	~LoginToSsiPortal();
	void init();
	void doLogin();
	void toGetProxyHost(const QString &mob);
	void setAcountName( const QString & );
	void setAcountPass( const QString & );
	void setSysConfig( SysConfig * );

	signals:
	void addtoLog( const QString & );
	void finished1( bool error, const QString &uri, const QString &errors = "" );
	void got_proxyinfo( bool error, const QString &name, const QString &str);
	void got_ssic(const QString &);
private:
	bool aborted;
	int httpGetId;
	QObject * parent;
	QBuffer * buffer;
	QTimer * timer;
	QString error_str, acount_name, acount_pass;
	SysConfig * syscfg;
	int step;
	QString get_uri( const QString &res );
	void get_proxyinfo( const QString &res );

private slots:
	void handle_request( int requestId, bool error );
	void handle_header( const QHttpResponseHeader &header );
	void handle_sslerrors( const QList<QSslError> &errors );
	void handle_timeout();
};

#endif // LOGINTOSSIPORTAL_H
