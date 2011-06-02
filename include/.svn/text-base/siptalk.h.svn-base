/*
 * siptalk.h
 *
 *  Created on: 2008-12-1
 *      Author: seerhut
 */

#ifndef SIPTALK_H_
#define SIPTALK_H_

#include <QObject>
#include <QByteArray>
#include <QString>
#include <QTcpSocket>
#include <QTimer>
#include <sipword.h>
#include "userinfo.h"

class SipTalk : public QTcpSocket
{
Q_OBJECT

public:

	SipTalk( QObject * parent );
	QObject * parent;
	void setUserInfo( UserInfo * );
	void do_connect();
	void send_word( const QString & );

	void test();

	signals:
	void addtoLog( const QString & );
	void finished( const QString &errors = "" );
	void connected_to_proxy();
	void received_register_word( SipWord * );
	void received_personal_info_word( SipWord * );
	void received_contacts_list_word( SipWord * );
	void received_group_list_word( SipWord * );
	void received_sub_presence_word( SipWord * );
	void received_contacts_info_word( SipWord * );
	void received_offline_messages_word( SipWord * );
	void received_score_word( SipWord * );

	void received_contact_specific_word( SipWord * );
private:
	SipWord * sipword;
	UserInfo * userinfo;
	//	QString word;
	QString pre_word;
	//	bool incomplete_word;
	void trans_by_callid( SipWord * );

private slots:
	//	void handle_request(int requestId, bool error);
	void handle_connected();
	void handle_disconnected();
	void handle_bytesWritten( qint64 );
	void handle_readyRead();
	void handle_error( QAbstractSocket::SocketError );
};

#endif /* SIPTALK_H_ */
