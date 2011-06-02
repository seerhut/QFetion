/*
 * siptalk.cpp
 *
 *  Created on: 2008-12-1
 *      Author: seerhut
 */
#include "siptalk.h"
#include <QStringList>
#include <QStringListIterator>

SipTalk::SipTalk( QObject * parent ) :
	QTcpSocket(parent)
{
	this->parent = parent;
	sipword = new SipWord(this);
	connect(sipword, SIGNAL(addtoLog(const QString &)), this, SLOT(addtoLog(const QString &)));

	connect(this, SIGNAL(connected()), this, SLOT(handle_connected()));
	connect(this, SIGNAL(disconnected()), this, SLOT(handle_disconnected()));
	connect(this, SIGNAL(bytesWritten(qint64)), this, SLOT(handle_bytesWritten(qint64)));
	connect(this, SIGNAL(readyRead()), this, SLOT(handle_readyRead()));
	connect(this, SIGNAL(error(QAbstractSocket::SocketError)),
	this, SLOT(handle_error(QAbstractSocket::SocketError)));
	qDebug() << "SipTalk Class Loaded ";
}

void SipTalk::do_connect()
{
	if( this->state() != SipTalk::ConnectedState )
	{
		this->connectToHost(userinfo->getProxyHost(), userinfo->getProxyPort());
		addtoLog(tr("connect to %1 %2").arg(userinfo->getProxyHost()).arg(
				userinfo->getProxyPort()));
	} else
	{
		connected();
	}
	//	waitForConnected(10000);
}

void SipTalk::send_word( const QString &word )
{

	QByteArray data;
	data.append(word);
	write(data);
	this->flush();
}
//
//void SipTalk::handle_request(int requestId, bool error)
//{
//
//
//}

void SipTalk::handle_connected()
{
	addtoLog("Sip: connected");
}

void SipTalk::handle_disconnected()
{
	addtoLog("Sip: disconnected");

}

void SipTalk::handle_bytesWritten( qint64 size )
{
	addtoLog("Sip: word sent");

}
void SipTalk::handle_error( QAbstractSocket::SocketError error )
{
	addtoLog(tr("Sip: error %1 : %2").arg(error).arg(this->errorString()));
	finished(this->errorString());

}

void SipTalk::handle_readyRead()
{
	int length;
	QString word, line, tmp, firstline, bigbody, body, head;
	word = readAll();
	addtoLog("SipTalk : handle_readyRead : new word arrived:");
	addtoLog("SipTalk : handle_readyRead : word start");
	addtoLog(word);
	addtoLog("SipTalk : handle_readyRead : word end");
	// must make sure that each 'word' is stated with valid firstline
	word.insert(0, pre_word);
	pre_word.clear();

	while( word.contains("\r\n\r\n") )
	{
		//there are complete firstline and heads here,
		tmp = word.section("\r\n\r\n", 0, 0);
		firstline = tmp.section("\r\n", 0, 0);
		head = tmp.section("\r\n", 1, -1);
		bigbody = word.section("\r\n\r\n", 1, -1);// may include next's head or other
		//body may be empty, but there must be a "\r\n\r\n"
		sipword->setFirstLine(firstline);
		QStringList list = head.split("\r\n", QString::KeepEmptyParts);
		QStringListIterator it(list);
		while( it.hasNext() )
		{//process each head line
			line = it.next();
			sipword->addHeadLine(line);
		}
		//process body
		length = sipword->getLength();
		if( bigbody.length() == length )
		{
			//normal single word
			addtoLog("SipTalk : handle_readyRead : normal pack");
			sipword->setBody(bigbody);
			trans_by_callid(sipword);
			sipword->init();
			return;
		}
		if( bigbody.length() < length )
		{
			//incomplete word
			addtoLog("SipTalk : handle_readyRead : incomplete pack");
			addtoLog(tr("SipTalk : handle_readyRead : %1 < %2").arg(
					bigbody.length()).arg(length));
			pre_word = word;
			sipword->init();
			return;
		}
		if( bigbody.length() > length )
		{
			//word contains next's some parts
			addtoLog("SipTalk : handle_readyRead : multi words pack");
			tmp = bigbody;
			tmp.truncate(length);
			sipword->setBody(tmp);
			trans_by_callid(sipword);
			sipword->init();
			bigbody.remove(0, length);
			word = bigbody;
			continue; //!!!!!!! do not jump out the word process circle
		}

	}
}

void SipTalk::trans_by_callid( SipWord *word )
{
	switch( word->getCallId() )
	{
		case SipWord::IdRegistry:
			addtoLog("SipTalk : trans_by_callid : Got IdRegistry");
			received_register_word(word);
			break;
		case SipWord::IdGetPersonalInfo:
			addtoLog("SipTalk : trans_by_callid : Got IdGetPersonalInfo");
			received_personal_info_word(word);
			break;
		case SipWord::IdGetContactsList:
			addtoLog("SipTalk : trans_by_callid : Got IdGetContactsList");
			received_contacts_list_word(word);
			break;
		case SipWord::IdGetGroupList:
			addtoLog("SipTalk : trans_by_callid : Got IdGetGroupList");
			received_group_list_word(word);
			break;
		case SipWord::IdSetPresence:
			addtoLog("SipTalk : trans_by_callid : Got IdSetPresence");
			//set presence response, no need to handle
			break;
		case SipWord::IdSubPresence:
			addtoLog("SipTalk : trans_by_callid : Got IdSubPresence");
			received_sub_presence_word(word);
			break;
		case SipWord::IdGetContactsInfo:
			addtoLog("SipTalk : trans_by_callid : Got IdGetContactsInfo");
			received_contacts_info_word(word);
			break;
		case SipWord::IdGetOfflineMessages:
			addtoLog("SipTalk : trans_by_callid : Got IdGetOfflineMessages");
			received_offline_messages_word(word);
			break;
		case SipWord::IdGetScore:
			addtoLog("SipTalk : trans_by_callid : Got IdGetScore");
			received_score_word(word);
			break;
		case 0://no Call Id
			addtoLog("SipTalk : trans_by_callid : Got NoCallId");
			//		addtoLog(word);
			break;
		default://call specific func
			if( !word->getFrom().isEmpty() || !word->getTo().isEmpty() )
			{
				received_contact_specific_word(word);
				break;
			}
			addtoLog(
					"SipTalk : trans_by_callid : Got Unknown CallId, will lookup registry");
			break;
	}
}

void SipTalk::setUserInfo( UserInfo *config )
{
	userinfo = config;
}

void SipTalk::test()
{

}
