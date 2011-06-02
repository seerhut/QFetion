#ifndef USERINFO_H
#define USERINFO_H

#include <QObject>
#include <QSettings>

class Utilities;
class SysConfig;
class QNetworkReply;
class LoginToSsiPortal;
class QByteArray;
class QNetworkAccessManager;
//class SipTalk;

class UserInfo : public QObject
{

Q_OBJECT

public:
	enum ssiStatus
	{
		loginToSSiPortal,
		loginToSSiPortalError,
		loginToSSiPortalDone,
		gettingProxyHost,
		getProxyHostDone,
		getProxyHostError,
	};
	enum userPresence
	{
		online = 400, offline, away = 100, busy = 600, shadow = 0
	};
	UserInfo( QObject * parent );
	void init();
	bool initFromConfig( const QString & );
	bool updateInfo( const QString & );
	bool saveToConfig();
	void setSysConfig( SysConfig * );
	//    void setSipTalk(SipTalk *);

	const QString &getSid();
	const QString &getUri();
	const QString &getMobNo();
	const QString &getPasswd();
	const QString &getNickname();
	const QString &getComplaint();
	const QString &getDomain();
	const QString &getProxyHost();
	const int getProxyPort();
	const QString &getProxyCnonce();
	const QByteArray &getPortrait();
	int getPresence();
	bool isSavePasswd();

	//    void setSid(const QString &);
	//    void setUri(const QString &);
	//    void setMobNo(const QString &);
	void setPasswd( const QString & );
	void setNickname( const QString & );
	void setComplaint( const QString & );
	void setCnonce( const QString & );
	void setDomain( const QString & );
	void setPresence( int );
	void setSavePasswd( bool );
	void setProxyHost( const QString & );
	void setProxyPort( int );
	void setProxyCnonce( const QString & );

	void requestUriFromSsiPortal();
	void requestProxyHost();
	void requestPortrait();


	signals:
	void addtoLog( const QString & );
	void ssiStatusChanged( UserInfo::ssiStatus );

private:
	QString cnonce, sid, uri, mobno, passwd, nickname, complaint, domain,
			proxyhost, ssic, get_portrait_url;
	bool savePasswd, autoLogin;
	int presence, lastPresence, proxyport;
	LoginToSsiPortal * loginssi;
	SysConfig * syscfg;
	QSettings settings;
	QByteArray portrait;
	QNetworkAccessManager *manager;

private slots:
	void handle_ssi_response( bool error, const QString &uri,
								const QString &errors = "" );
	void handle_proxy_response( bool error, const QString &uri,
									const QString &errors = "" );
	void handle_got_ssic(const QString &);
	void handle_getPortrait(QNetworkReply*);

};

#endif // USERINFO_H
