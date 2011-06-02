#ifndef CONTACT_H
#define CONTACT_H

#include <QWidget>
#include <QThread>
#include "message.h"
#include "control.h"
#include <QSettings>

class ChatWindow;
class ContactListItem;
class QDomElement;
class UserInfo;

class Contact : public QWidget
{
Q_OBJECT

public:
	enum clientType
	{
		clientPC, clientSMS, clientOFFLINE, clientUnknown
	};
	enum presenceType
	{
		presenceOnline = 400,
		presenceOffline = 0,
		presenceBusy = 600,
		presenceAway = 100
	};

	enum communicateTtpe
	{
		comIviting, comIvited, comBeInvited, comBeInviting, comNoCom
	};
	Contact( QWidget * parent = 0 );
	void init( const QString &uri, int team );
	void setUserInfo( UserInfo * );
	void setCallId(int);

	void setMobNo( const QString & );
	void setComplaint( const QString & );
	void setNickname( const QString & );
	void setTeam( int );
	void setPresence( int );
	void setClientType( int );

	const QString &getUri();
	const QString &getSid();
	const QString &getMobNo();
	const QString &getComplaint();
	const QString &getNickname();
	int getTeam();
	int getPresence();
	int getClientType();

	void do_invite();
	void do_bye();

	bool updateInfo( const QDomElement &contactelem );
	void updatePresence( const QDomElement &presenceelem );
	void updateShow();

	ContactListItem * getListItem();
	Message getMessage();
	void addMessage( Message );
	void handle_word( SipWord * word );
	void handle_invite_word( SipWord * word );
	void handle_options_word( SipWord * word );
	void handle_bye_word( SipWord * word );
	void handle_msg_word( SipWord * word );

public slots:
	ChatWindow * getChatWindow();//if no window, create one
	void showChatWindow();
	void sendMsg( const QString & );
	void sendSms( const QString & );
	void cwshow();
	void cwhide();

	signals:
	//	void contactWantToUpdateInfo(const QString &uri);
	void send_word( const QString & );

private:

	QString uri;
	QString sid;
	QString mobno;
	QString nickname;
	QString complaint;
	QString domain;
	int team;
	QObject * parent;
	UserInfo * userinfo;
	int presence;
	QString presence_comment;
	ChatWindow * chatWindow;
	ContactListItem * listitem;
	QList<Message> msg_buffer;
	QSettings settings;
	int client_type;
	int callId;
	int backup_callId;
	int cseq;
	bool isInvtited;

};

#endif // CONTACT_H
