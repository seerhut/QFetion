#include "contact.h"
#include "chatwindow.h"
#include "contactlistitem.h"
#include <QDebug>
#include <QDomElement>
#include <QMouseEvent>
#include "utilities.h"
#include "userinfo.h"
Contact::Contact( QWidget * parent ) :
	QWidget(parent)
{
	this->parent = parent;
	chatWindow = 0;
	//	callId = 0;
	cseq = 1;
	listitem = new ContactListItem(this);
	connect(listitem, SIGNAL(doubleClicked()), this, SLOT(showChatWindow()));
	qDebug() << "Contact : contact created";
	this->hide();
}

void Contact::init( const QString &uri, int team )
{
	this->uri = uri;
	this->team = team;
	isInvtited = false;
	//at startup if no config file , only show sid;

	settings.beginGroup(uri);
	domain = settings.value("domain", "fetion.com.cn").toString();
	sid = settings.value("sid", Utilities::getSidFromUri(uri)).toString();
	complaint = settings.value("complaint", "").toString();
	nickname = settings.value("nickname", "").toString();
	settings.endGroup();

	updateShow();

	qDebug() << "contact init : " << sid;
}

void Contact::showChatWindow()
{
	getChatWindow()->show();
	updateShow();
}

ChatWindow * Contact::getChatWindow()
{
	if( !chatWindow )
	{
		chatWindow = new ChatWindow(this);
		//		chatWindow->setClient(false);
		updateShow();
		connect(chatWindow, SIGNAL(send_text_client(const QString &)), this,
				SLOT(sendMsg( const QString & )));
		connect(chatWindow, SIGNAL(send_text_sms(const QString &)), this,
				SLOT(sendSms( const QString & )));
		connect(chatWindow, SIGNAL(illshow()), this, SLOT(cwshow()));
		connect(chatWindow, SIGNAL(illhide()), this, SLOT(cwhide()));
	}
	return chatWindow;
}

ContactListItem * Contact::getListItem()
{
	return listitem;
}

void Contact::updateShow()
{
	//update chatwindow
	if( chatWindow )
	{
		chatWindow->setWindowTitle(nickname + " -  " + complaint);
		chatWindow->setContactSid(sid);
		chatWindow->setContactMobno(mobno);
		if( (presence == Contact::presenceOnline) || (presence
				== Contact::presenceAway)
				|| (presence == Contact::presenceBusy) )
		{
			chatWindow->setClient(true);
		} else
		{
			chatWindow->setClient(false);
		}
	}

	//update list item

	listitem->setNickName(nickname);
	listitem->setComplaint(complaint);
	QString desc;
	if( !presence_comment.isEmpty() )
	{
		desc = "(" + presence_comment + ")";
	}

	switch( presence )
	{
		case Contact::presenceOnline:
			listitem->setPresence("Online" + desc);
			listitem->setPresenceIcon(":/client ena");
			break;
		case Contact::presenceOffline:
			listitem->setPresence("SMS" + desc);
			listitem->setPresenceIcon(":/sms ena");
			break;
		case Contact::presenceAway:
			listitem->setPresence("Away" + desc);
			listitem->setPresenceIcon(":/client ena");
			break;
		case Contact::presenceBusy:
			listitem->setPresence("Busy" + desc);
			listitem->setPresenceIcon(":/client ena");
			break;
		default:
			listitem->setPresence("Unknown" + desc);
			listitem->setPresenceIcon(":/sms dis");
			break;
	}
	settings.beginGroup(uri);
	settings.setValue("domain", domain);
	settings.setValue("sid", sid);
	settings.setValue("complaint", complaint);
	settings.setValue("nickname", nickname);
	settings.endGroup();
}

void Contact::updatePresence( const QDomElement &presenceelem )
{
	/* <presence uri="sip:756444636@fetion.com.cn;p=1414">
	 <basic device-id="" value="0" desc="" device-type="" device-caps="" />
	 <personal nickname="......" impresa=":qw" portrait-crc="598224859" />
	 <extended type="sms">0.0:0:0</extended>
	 <extended type="listening"></extended>
	 <extended type="location"><location city="" distance="" /></extended>
	 </presence>*/
	if( presenceelem.attribute("uri") != this->uri )
	{
		qDebug() << "Contact : updateInfo : got wrong info (elem)";
		return;
	}
	QDomElement elem = presenceelem;
	if( !presenceelem.firstChildElement("basic").isNull() )
	{// contact personal info
		elem = elem.firstChildElement("basic");
		if( !elem.isNull() )
		{

			presence = elem.attribute("value").toInt();
			presence_comment = elem.attribute("desc");
			if( presence == 0 ) isInvtited = false;
			//TODO set other info

			qDebug() << "Contact : updateInfo :" << sid << " presence : "
					<< presence;
			updateShow();
		} else
		{
			qDebug() << "Contact : updateInfo : no personal info";
		}
	}
	if( !presenceelem.firstChildElement("personal").isNull() )
	{// contact personal info
		elem = elem.firstChildElement("personal");
		if( !elem.isNull() )
		{
			nickname = elem.attribute("nickname");
			complaint = elem.attribute("impresa");
			//TODO set other info

			qDebug() << "Contact : updateInfo :" << sid
					<< " found personal info";
			qDebug() << "Contact : updateInfo :" << sid << " nick : "
					<< nickname;
			qDebug() << "Contact : updateInfo :" << sid << " comp : "
					<< complaint;
			updateShow();
		} else
		{
			qDebug() << "Contact : updateInfo : no personal info";
		}
	}
}

bool Contact::updateInfo( const QDomElement &contactelem )
{
	/*
	 <contact uri="sip:585195198@fetion.com.cn;p=5005" status-code="200">
	 <personal version="20" nickname="........." gender="2" impresa=".................."
	 ivr-enabled="1" portrait-crc="2074595345" provisioning="1" mobile-no="13910423555" name=""/>
	 </contact>

	 <contact uri="sip:756444636@fetion.com.cn;p=1414"><extended score-level="1"/></contact>
	 */

	if( contactelem.attribute("uri") != this->uri )
	{
		qDebug() << "Contact : updateInfo : got wrong info (elem)";
		return false;
	}
	QDomElement elem = contactelem;
	//found own info
	if( !elem.firstChildElement("personal").isNull() )
	{// contact personal info
		elem = elem.firstChildElement("personal");
		if( !elem.isNull() )
		{
			nickname = elem.attribute("nickname");
			complaint = elem.attribute("impresa");
			mobno = elem.attribute("mobile-no");
			//TODO set other info

			qDebug() << "Contact : updateInfo :" << sid
					<< " found personal info";
			qDebug() << "Contact : updateInfo :" << sid << " nick : "
					<< nickname;
			qDebug() << "Contact : updateInfo :" << sid << " comp : "
					<< complaint;
			updateShow();
			return true;
		} else
		{
			qDebug() << "Contact : updateInfo : no personal info";
			return false;
		}
	} else if( !elem.firstChildElement("extended").isNull() )
	{//extended info
		elem = elem.firstChildElement("extended");
		if( elem.hasAttribute("score-level") )
		{
			//TODO set score-level
			qDebug() << "Contact : updateInfo :" << sid << "found extened info";
			updateShow();
			return true;
		} else
		{
			qDebug() << "Contact : updateInfo :" << sid
					<< "extened has no score attr";
			return false;
		}

	}
	qDebug() << "Contact : updateInfo :" << sid << " found nothing";
	return false;
}

void Contact::handle_word( SipWord * word )
{
	// recognize word and send to proper handle
	switch( word->getType() )
	{
		case SipWord::TypeInvite:
		case SipWord::TypeAck:
			handle_invite_word(word);
			break;
		case SipWord::TypeBye:
			handle_bye_word(word);
			break;
		case SipWord::TypeMessage:
			handle_msg_word(word);
			break;
		case SipWord::TypeOptions:
			handle_options_word(word);
			break;
		default:
			//strange
			break;
	}

}

void Contact::do_invite()
{
	callId = backup_callId;
	cseq = 1;
	QString body = tr("v=0\n"
		"o=-0 0 IN 127.0.0.1:1084\n"
		"s=session\n"
		"c=IN IP4 127.0.0.1:1084\n"
		"t=0 0\n"
		"m=message 1084 sip %1").arg(userinfo->getUri());

	send_word(tr("I fetion.com.cn SIP-C/2.0\r\n"
		"F: %1\r\n"
		"I: %2 \r\n"
		"Q: 1 I\r\n"
		"T: %3\r\n"
		"K: text/plain\r\n"
		//		"K: multiparty\r\n"
			//		"K: nudge\r\n"
			"L: %4\r\n"
			"\r\n") .arg(userinfo->getSid()) .arg(callId) .arg(uri) .arg(
			body.toLocal8Bit().length()) + body);
	getChatWindow()->add_sys_msg("inviting contact ....");
}

void Contact::do_bye()
{/*B fetion.com.cn SIP-C/2.0
 F: 678345136
 I: 36
 Q: 5 B
 T: sip:585195198@fetion.com.cn;p=5005
 */
	(cseq > 0) ? cseq++ : cseq--;
	send_word(tr("B fetion.com.cn SIP-C/2.0\r\n"
		"F: %1\r\n"
		"I: %2 \r\n"
		"Q: %3 B\r\n"
		"T: %4\r\n"
		"\r\n") .arg(userinfo->getSid()) .arg(callId) .arg(cseq).arg(uri));
	callId = backup_callId;
	cseq = 1;
	getChatWindow()->add_sys_msg("say bye ....");
}

void Contact::handle_invite_word( SipWord * word )
{
	QString body;
	switch( word->getStatusCode() )
	{
		case SipWord::CodeOk:
			// invite ok
			send_word(tr("A fetion.com.cn SIP-C/2.0\r\n"
				"I: %1 \r\n"
				"Q: 1 A\r\n"
				"T: %2\r\n"
				"F: %3\r\n\r\n").arg(callId).arg(uri).arg(userinfo->getSid()));
			getChatWindow()->add_sys_msg("inviting contact done");

			isInvtited = true;
			break;
		case SipWord::CodeTry:
			//inviting....not need to handle
			getChatWindow()->add_sys_msg("server is waiting ....");

			break;
		case SipWord::CodeServerTimeOut:
			getChatWindow()->add_sys_msg("inviting timeout!");
			isInvtited = false;
			break;
		case SipWord::CodeNone:
			//contact invite me
			/*I 678345136 SIP-C/2.0
			 Q: 1 I
			 F: sip:585195198@fetion.com.cn;p=5005
			 K: text/html-fragment
			 K: multiparty
			 K: nudge
			 I: -8
			 L: 129

			 v=0
			 o=-0 0 IN 127.0.0.1:4441
			 s=session
			 c=IN IP4 127.0.0.1:4441
			 t=0 0
			 m=message 4441 sip sip:585195198@fetion.com.cn;p=5005

			 SIP-C/2.0 200 OK
			 I: -8
			 Q: 1 I
			 F: sip:585195198@fetion.com.cn;p=5005
			 K: text/html-fragment
			 K: multiparty
			 K: nudge
			 L: 129

			 v=0
			 o=-0 0 IN 127.0.0.1:1208
			 s=session
			 c=IN IP4 127.0.0.1:1208
			 t=0 0
			 m=message 1208 sip sip:678345136@fetion.com.cn;p=5017

			 A 678345136 SIP-C/2.0
			 Q: 1 A
			 F: sip:585195198@fetion.com.cn;p=5005
			 I: -8*/
			if( word->getType() == SipWord::TypeAck )
			{
				getChatWindow()->add_sys_msg("session built");
				isInvtited = true;
				return;
			}
			callId = word->getCallId();
			cseq = word->getCSeq();
			body = tr("v=0\r\n"
				"o=-0 0 IN 127.0.0.1:1208\r\n"
				"s=session\r\n"
				"c=IN IP4 127.0.0.1:1208\r\n"
				"t=0 0\r\n"
				"m=message 1208 sip %1").arg(userinfo->getUri());
			send_word(tr("SIP-C/2.0 200 OK\r\n"
				"I: %1\r\n"
				"Q: %2 I\r\n"
				"F: %3\r\n"
				"K: text/plain\r\n"
				//						"K: multiparty\r\n"
					//						"K: nudge\r\n"
					"L: %4\r\n"
					"\r\n") .arg(callId).arg(cseq) .arg(uri) .arg(
					body.toLocal8Bit().length()) + body);
			getChatWindow()->add_sys_msg("got invite");

			break;
		default:
			//unknown code
			getChatWindow()->add_sys_msg(word->getFirstLine());
			break;
	}
}

void Contact::handle_bye_word( SipWord * word )
{
	switch( word->getStatusCode() )
	{
		case SipWord::CodeOk:
			//			getChatWindow()->add_sys_msg("");
			break;
		case SipWord::CodeNone:
			// got bye..
			/*B fetion.com.cn SIP-C/2.0
			 F: 678345136
			 I: 36
			 Q: 5 B
			 T: sip:585195198@fetion.com.cn;p=5005

			 SIP-C/2.0 200 OK
			 Q: 5 B
			 F: sip:585195198@fetion.com.cn;p=5005
			 I: 36*/
			send_word(tr("SIP-C/2.0 200 OK\r\n"
				"I: %1\r\n"
				"Q: %2 B\r\n"
				"F: %3\r\n"
				"\r\n") .arg(callId) .arg(cseq).arg(userinfo->getUri()));
			getChatWindow()->add_sys_msg("got bye");
			callId = backup_callId;

			break;
		default:
			//unknown code
			getChatWindow()->add_sys_msg(word->getFirstLine());
			break;
	}
}

void Contact::handle_msg_word( SipWord * word )
{
	switch( word->getStatusCode() )
	{
		case SipWord::CodeOk:
			getChatWindow()->add_sys_msg("Msg send ok");
			break;
		case SipWord::CodeSendSMSOk:
			getChatWindow()->add_sys_msg("SMS send ok");
			break;
		case SipWord::CodeNone:
			/*M 678345136 SIP-C/2.0
			 C: text/html-fragment
			 Q: 2 M
			 D: Wed, 03 Dec 2008 06:36:27 GMT
			 F: sip:585195198@fetion.com.cn;p=5005
			 K: SaveHistory
			 I: -8
			 XI: 270da50dcb7e4b239ad3c936da2fc90b
			 L: 66

			 <Font Face='......' Color='-16777216' Size='10.5'>.........</Font>

			 SIP-C/2.0 200 OK
			 F: sip:585195198@fetion.com.cn;p=5005
			 I: -8
			 Q: 2 M*/
			send_word(tr("SIP-C/2.0 200 OK\r\n"
				"F: %1\r\n"
				"I: %2\r\n"
				"Q: %3 M\r\n\r\n") .arg(uri) .arg(word->getCallId()) .arg(
					word->getCSeq()));
			cseq = word->getCSeq();
			getChatWindow()->show();

			getChatWindow()->add_in_msg(nickname, word->getBody());
			break;
		default:
			getChatWindow()->add_sys_msg(word->getFirstLine());
			//unknown code
			break;
	}
}

void Contact::handle_options_word( SipWord * word )
{
	switch( word->getStatusCode() )
	{
		case SipWord::CodeNone:
			/*SIP-C/2.0 200 OK
			 F: sip:678345136@fetion.com.cn;p=5017
			 I: 17
			 Q: 3 O
			 K: text/html-fragment
			 K: text/plain*/
			send_word(tr("SIP-C/2.0 200 OK\r\n"
				"F: %1\r\n"
				"I: %2\r\n"
				"Q: %3 O\r\n"
				"K: text/plain\r\n"
				"K: text/html-fragment\r\n\r\n") .arg(uri) .arg(
					word->getCallId()) .arg(word->getCSeq()));
			break;
		default:
			getChatWindow()->add_sys_msg(word->getFirstLine());
			//unknown code
			break;
	}
}

void Contact::sendMsg( const QString &str )
{
	(cseq > 0) ? cseq++ : cseq--;
	send_word(
			tr("M fetion.com.cn SIP-C/2.0\r\n"
				"F: %1\r\n"
				"I: %2 \r\n"
				"Q: %3 M\r\n"
				"T: %4\r\n"
				//		"C: text/html-fragment\r\n"
					"C: text/plain\r\n"
					//	"K: SaveHistory\r\n"
					"L: %5\r\n"
					"\r\n").arg(userinfo->getSid()) .arg(callId) .arg(cseq) .arg(
					uri) .arg(str.toUtf8().length()) + str.toUtf8());
	getChatWindow()->add_out_msg(userinfo->getNickname(), str);
	cseq++;
}

void Contact::sendSms( const QString &str )
{
	(cseq > 0) ? cseq++ : cseq--;
	send_word(
			tr("M fetion.com.cn SIP-C/2.0\r\n"
				"F: %1\r\n"
				"I: %2 \r\n"
				"Q: %3 M\r\n"
				"T: %4\r\n"
				"N: SendSMS\r\n"
				"C: text/plain\r\n"
				//		"K: SaveHistory\r\n"
					"L: %5\r\n"
					"\r\n").arg(userinfo->getSid()) .arg(callId) .arg(cseq) .arg(
					uri) .arg(str.toUtf8().length()) + str.toUtf8());
	getChatWindow()->add_out_msg(userinfo->getNickname(), str);
	cseq++;
}
//void Contact::setUri(const QString &str)
//{
//	uri = str;
//}
//void Contact::setSid(const QString &str)
//{
//	sid = str;
//}

void Contact::cwshow()
{
	if( !isInvtited && (presence != presenceOffline) )
	{
		do_invite();
	} else
	{

	}
}

void Contact::cwhide()
{
	if( isInvtited && (presence != presenceOffline) )
	{
		do_bye();
	} else
	{

	}
}

void Contact::setUserInfo( UserInfo *u )
{
	userinfo = u;
}

void Contact::setCallId( int id )
{
	backup_callId = id;
	callId = id;
}

void Contact::setMobNo( const QString &str )
{
	mobno = str;
}
void Contact::setComplaint( const QString &str )
{
	complaint = str;
}
void Contact::setNickname( const QString &str )
{
	nickname = str;
}
void Contact::setTeam( int id )
{
	team = id;
}
void Contact::setPresence( int type )
{
	presence = type;
}
void Contact::setClientType( int type )
{
	client_type = type;
}

const QString &Contact::getUri()
{
	return uri;
}
const QString &Contact::getSid()
{
	return sid;
}
const QString &Contact::getMobNo()
{
	return mobno;
}
const QString &Contact::getComplaint()
{
	return complaint;
}
const QString &Contact::getNickname()
{
	return nickname;
}
int Contact::getTeam()
{
	return team;
}
int Contact::getPresence()
{
	return presence;
}
int Contact::getClientType()
{
	return client_type;
}

