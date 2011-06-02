#ifndef CONTROL_H
#define CONTROL_H

#include <QObject>
#include <QDebug>
#include <QWidget>
#include <QByteArray>
#include "siptalk.h"
#include "userinfo.h"
#include "sipword.h"
class Contact;
//class MainWindow;
//class QTreeWidget;
class SysConfig;
class Utilities;
class QTimer;

class Control : public QWidget
{
Q_OBJECT

public:
	enum loginStatus
	{
		notLogin,
		loginToSSiPortal,
		loginToSSiPortalError,
		loginToSSiPortalDone,
		getProxyHost,
		getProxyHostDone,
		getProxyHostError,
		registerToProxyConnecting,
		registerToProxyConnected,
		registerToProxyRequestNonce,
		registerToProxySentDigest,
		registerToProxyError,
		registerToProxyDone,
		getUserInfo,
		getUserInfoDone,
		getUserInfoError,
		getContacts,
		getContactsDone,
		getContactsError,
		shouldShowContactsPage,
		logoutFromProxy,
		logoutFromProxyDone,
		serverCloseConnection
	};

	Control( QWidget *parent = 0 );
	~Control();

	bool hasLogin();
	//	void setMainWindow(MainWindow *);
	//	void setContactsTree(QTreeWidget *);
	void test();
	QHash<QString, Contact *> &getContactsHash();
	QHash<int, QString> &getGroupsHash();
	SysConfig * syscfg;
	UserInfo * userinfo;

protected:
	void run();

	signals:
	void addtoLog( const QString & );
	void sbMsg( const QString &, int = 2000 );
	void loginStatusChanged( Control::loginStatus );

private:
	int loginstatus;
	SipTalk * siptalk;
	QString nonce;
	QHash<QString, Contact *> contactsHash;

	QHash<int, QString> groupName;
	QTimer * katimer;
	int reReg, cseq_setpresence,cseq_reg;

	void loginToFetionProxy();
	void request_nonce();
	void setLoginStatus( Control::loginStatus );
	void send_digest();
	void get_personal_info();
	void get_contacts_list();
	void get_groups_list();
	void get_contacts_info( const QString &uri = "" );
	void sub_contacts_precense();
	//	void sub_contacts_precense();


	bool update_contacts_list( const QString & );

public slots:
	void change_presence( const QString & );
	void tologin();

	void tologout();

	void handle_sip_register( SipWord * );
	void handle_sip_personal_info( SipWord * );
	void handle_sip_contacts_list( SipWord * );
	void handle_sip_connected();
	void handle_sip_disconnected();
	void handle_sip_error( const QString & );
	void handle_contact_update_request( const QString &uri );
	void handle_sip_contacts_info( SipWord * );
	void handle_sip_contacts_precense( SipWord * );
	void handle_sip_contact_specific_precense( SipWord * );
	void handle_userinfo_ssiStatusChanged( UserInfo::ssiStatus );
	void handle_contact_send_word( const QString & );
	void keep_alive();
	//temp
};

#endif // CONTROL_H
