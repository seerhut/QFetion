#include "control.h"
#include "contact.h"
#include "sysconfig.h"
#include "utilities.h"
#include "mainwindow.h"
#include "chatwindow.h"
#include <QDomDocument>
#include <QDomNodeList>
#include <QDomNode>
#include <QTimer>
Control::Control( QWidget * parent ) :
	QWidget(parent)
{
	//	this->parent = parent;
	loginstatus = Control::notLogin;

	cseq_setpresence = 1;

	//sysconfig
	syscfg = new SysConfig(this);
	syscfg->initFromConfig();

	//userinfo
	userinfo = new UserInfo(this);
	userinfo->setSysConfig(syscfg);

	//load siptalk interact service
	siptalk = new SipTalk(this);
	siptalk->setUserInfo(userinfo);

	//
	katimer = new QTimer(this);

	connect(userinfo, SIGNAL(addtoLog(const QString &)), this,
			SIGNAL(addtoLog(const QString &)));
	connect(syscfg, SIGNAL(addtoLog(const QString &)), this,
			SIGNAL(addtoLog(const QString &)));
	connect(siptalk, SIGNAL(addtoLog(const QString &)), this,
			SIGNAL(addtoLog(const QString &)));
	//-----
	connect(userinfo, SIGNAL(ssiStatusChanged(UserInfo::ssiStatus)), this,
			SLOT(handle_userinfo_ssiStatusChanged(UserInfo::ssiStatus)));

	connect(siptalk, SIGNAL(received_register_word(SipWord *)), this,
			SLOT(handle_sip_register(SipWord *)));
	connect(siptalk, SIGNAL(received_personal_info_word(SipWord *)), this,
			SLOT(handle_sip_personal_info(SipWord *)));
	connect(siptalk, SIGNAL(received_contacts_list_word(SipWord *)), this,
			SLOT(handle_sip_contacts_list(SipWord *)));
	connect(siptalk, SIGNAL(received_contacts_info_word(SipWord *)), this,
			SLOT(handle_sip_contacts_info(SipWord *)));
	connect(siptalk, SIGNAL(received_sub_presence_word(SipWord *)), this,
			SLOT(handle_sip_contacts_precense(SipWord *)));
	connect(siptalk, SIGNAL(received_contact_specific_word(SipWord *)), this,
			SLOT(handle_sip_contact_specific_precense(SipWord *)));

	connect(siptalk, SIGNAL(finished(const QString &)), this,
			SLOT(handle_sip_error(const QString &)));
	connect(siptalk, SIGNAL(connected()), this, SLOT(handle_sip_connected()));
	connect(siptalk, SIGNAL(disconnected()), this,
			SLOT(handle_sip_disconnected()));

	connect(katimer, SIGNAL(timeout()), this, SLOT(keep_alive()));

	qDebug() << "Control Class Loaded ";
	addtoLog(tr("中文测试"));
	qDebug() << tr("中文测试");
	this->hide();
}

Control::~Control()
{
	qDebug() << "Control Class unloaded ";
}

//void Control::setMainWindow(MainWindow *m)
//{
//	mainwindow = m;
//}

//void Control::setContactsTree(QTreeWidget *tree)
//{
//	contactstree = tree;
//}

/*************************************************
 * login entry
 * just choose action by checking if we already has uri
 *************************************************/

void Control::tologin()
{
	qDebug() << "Control : to login";
	setLoginStatus(Control::notLogin);
	reReg = 0;
	if( !userinfo->getUri().isEmpty() && !userinfo->getSid().isEmpty()
			&& !userinfo->getProxyHost().isEmpty() )
	{
		//		if(!userinfo->getProxyHost().isEmpty()){
		loginToFetionProxy();
		//		}else{
		//			userinfo->requestProxyHost();
		//			setLoginStatus(Control::getProxyHost);
		//		}
	} else
	{
		userinfo->requestUriFromSsiPortal();
	}
	return;
}
/*************************************************
 * begin to connect and got to register to proxy
 *************************************************/
void Control::loginToFetionProxy()
{
	addtoLog(tr("Control: to login to fetion proxy"));

	addtoLog("Control: got id : " + userinfo->getSid());
	setLoginStatus(Control::registerToProxyConnecting);
	siptalk->do_connect();
	return;
}

void Control::request_nonce()
{
	addtoLog(tr("Control: to Request nonce"));
	setLoginStatus(Control::registerToProxyRequestNonce);
	siptalk->send_word(tr("R fetion.com.cn SIP-C/2.0\r\n"
		"F: %1\r\n"
		"I: %2 \r\n"
		"Q: 1 R\r\n"
		"L: %3\r\n"
		"\r\n").arg(userinfo->getSid()).arg(SipWord::IdRegistry).arg(
			syscfg->getHelloStr().length()) + syscfg->getHelloStr());
	cseq_reg = 2;
	siptalk->flush();
}

void Control::send_digest()
{
	QString response = Utilities::calculateDigest(nonce, userinfo->getUri(),
			userinfo->getSid(), userinfo->getPasswd(),
			userinfo->getProxyCnonce(), userinfo->getDomain());
	addtoLog(tr("Control: send digest response"));
	setLoginStatus(Control::registerToProxySentDigest);
	siptalk->send_word(tr("R fetion.com.cn SIP-C/2.0\r\n"
		"F: %1\r\n"
		"I: %2 \r\n"
		"Q: 2 R\r\n"
		"A: Digest algorithm=\"MD5-sess\", response=\"%3\", cnonce=\"%4\"\r\n"
		//			"salt=\"%4\"\r\n"
			"L: %5\r\n").arg(userinfo->getSid()).arg(SipWord::IdRegistry).arg(
			response) .arg(userinfo->getProxyCnonce()).arg(
			syscfg->getHelloStr().length()) + "\r\n" + syscfg->getHelloStr());
	siptalk->flush();
}

void Control::handle_sip_register( SipWord *word )
{
	//algorithm="MD5-sess;SHA1-sess",nonce="115D841444272AA115CEFF255BD78468"
	switch( word->getStatusCode() )
	{
		case SipWord::CodeUnauthorized:
			switch( loginstatus )
			{
				case Control::registerToProxyRequestNonce:

					nonce = word->getWWWAuth().section("nonce=", 1, 1).section(
							"\"", 1, 1);
					addtoLog("nonce :" + nonce);
					send_digest();

					break;
				case Control::registerToProxySentDigest:

					if( reReg < 2 )
					{
						//						sleep(2);
						//						request_nonce();
						addtoLog("Control: get 401 again, retry..");
						reReg++;
					} else
					{
						addtoLog("Control: get 401 again, register error");
						setLoginStatus(Control::registerToProxyError);
						siptalk->abort();
					}

					break;
			}
		case SipWord::CodeOk:
			if( loginstatus == Control::registerToProxySentDigest )
			{
				setLoginStatus(Control::registerToProxyDone);
				//register successfully
				katimer->start(30000);
				userinfo->saveToConfig();
				get_personal_info();
			}
			addtoLog("Control: Got 200 OK ");
			break;
		case SipWord::CodeBadRequest:
			//			sleep(2);
			setLoginStatus(Control::registerToProxyError);
			abort();
			//			tologin();
			break;
		default:
			addtoLog("Control: Got unknown answer: ");
			setLoginStatus(Control::registerToProxyError);
			break;
	}

}

/*************************************************
 * normal sip status
 *************************************************/

void Control::handle_sip_connected()
{
	setLoginStatus(Control::registerToProxyConnected);
	addtoLog("Control : connected to proxy");
	request_nonce();
}

void Control::handle_sip_disconnected()
{
	Contact * c;
	foreach(c, contactsHash)
		{
			delete c;
		}
	groupName.clear();
	contactsHash.clear();
	katimer->stop();
	setLoginStatus(Control::logoutFromProxy);
	setLoginStatus(Control::notLogin);
	setLoginStatus(Control::serverCloseConnection);
	addtoLog("Control : disconnected from proxy");
}

void Control::handle_sip_error( const QString &error )
{
	addtoLog(tr("Control : connected to proxy").arg(error));
	sbMsg("login to proxy error", 2000);
	setLoginStatus(Control::registerToProxyError);
	siptalk->abort();
}
/*************************************************
 * status change
 *************************************************/
void Control::change_presence( const QString &comment )
{
	addtoLog(tr("Control: I'll change online status to : %1").arg(
			userinfo->getPresence()));
	QString body = tr(
			"<args><presence><basic value=\"%1\" /></presence></args>").arg(
			userinfo->getPresence());
	siptalk->send_word(tr("S fetion.com.cn SIP-C/2.0\r\n"
		"F: %1\r\n"
		"I: %2 \r\n"
		"Q: %3 S\r\n"
		"N: SetPresence\r\n"
		"L: %4\r\n"
		"\r\n") .arg(userinfo->getSid()).arg(SipWord::IdSetPresence).arg(
			cseq_setpresence).arg(body.toUtf8().length()) + body.toUtf8());
	cseq_setpresence++;
	addtoLog("Control: Set Presence ");
}

/*************************************************
 * Get Personal Info
 *************************************************/
void Control::get_personal_info()
{
	addtoLog(tr("Control: to get personal info"));
	setLoginStatus(Control::getUserInfo);
	QString
			body =
					tr(
							"<args><personal attributes=\"all\" /><services version=\"\" attributes=\"all\" />"
								"<config version=\"12\" attributes=\"all\" /><mobile-device attributes=\"all\" /></args>");
	siptalk->send_word(tr("S fetion.com.cn SIP-C/2.0\r\n"
		"F: %1\r\n"
		"I: %2 \r\n"
		"Q: 1 S\r\n"
		"N: GetPersonalInfo\r\n"
		"L: %3\r\n"
		"\r\n").arg(userinfo->getSid()) .arg(SipWord::IdGetPersonalInfo) .arg(
			body.toLocal8Bit().length()) + body);
}

void Control::handle_sip_personal_info( SipWord *word )
{
	switch( word->getStatusCode() )
	{
		case SipWord::CodeUnauthorized:
			addtoLog("Control: lost registry ");
			break;
		case SipWord::CodeOk:
			userinfo->updateInfo(word->getBody());
			addtoLog("Control: Got 200 OK ");
			setLoginStatus(Control::getUserInfoDone);
			get_contacts_list();
			break;
		default:
			addtoLog("Control: Got unknown answer: ");
			break;
	}
}

/*************************************************
 * log out
 *************************************************/
void Control::get_contacts_list()
{
	addtoLog(tr("Control: to get contacts list"));
	setLoginStatus(Control::getContacts);
	QString body = tr(
			"<args><contacts><buddy-lists /><buddies attributes=\"all\" />"
				"<mobile-buddies attributes=\"all\" />"
				"<chat-friends /><blacklist /></contacts></args>");
	siptalk->send_word(tr("S fetion.com.cn SIP-C/2.0\r\n"
		"F: %1\r\n"
		"I: %2 \r\n"
		"Q: 1 S\r\n"
		"N: GetContactList\r\n"
		"L: %3\r\n"
		"\r\n") .arg(userinfo->getSid()) .arg(SipWord::IdGetContactsList) .arg(
			body.toLocal8Bit().length()) + body);
	siptalk->flush();
}
void Control::handle_sip_contacts_list( SipWord *word )
{

	if( word->getStatusCode() == SipWord::CodeOk )
	{
		if( update_contacts_list(word->getBody()) )
		{
			addtoLog(
					"Control : handle_sip_get_contacts_list :  Got Contacts list Done");
			setLoginStatus(Control::getContactsDone);
			get_contacts_info("");
			//			sleep(1);
			sub_contacts_precense();
		} else
		{
			//			sleep(1);
			get_contacts_list();
			addtoLog(
					"Control : handle_sip_get_contacts_list :  get contacts list, but parse failed");
			//			setLoginStatus(Control::getContactsError);
		}
	} else
	{
		//		sleep(1);
		get_contacts_list();
		addtoLog("Control : handle_sip_get_contacts_list :  not 200 ok");
		//		setLoginStatus(Control::getContactsError);
	}
}

bool Control::update_contacts_list( const QString &body )
{
	/*SIP-C/2.0 200 OK
	 I: 3
	 Q: 1 S
	 L: 557

	 <results><contacts version="13">
	 <buddy-lists>
	 <buddy-list id="1" name="......" />
	 <buddy-list id="2" name="......" />
	 </buddy-lists>
	 <buddies>
	 <buddy uri="sip:585195198@fetion.com.cn;p=5005" local-name="" buddy-lists="2" relation-status="1" online-notify="1" />
	 <buddy uri="sip:756444636@fetion.com.cn;p=1414" local-name="" buddy-lists="2" relation-status="1" online-notify="1" />
	 <buddy uri="sip:758936988@fetion.com.cn;p=1082" local-name="" buddy-lists="1" relation-status="1" online-notify="0" />
	 </buddies>
	 <mobile-buddies />
	 <chat-friends />
	 </contacts></results>*/
	addtoLog("Control : update_contacts_list :  to parse contacts list word");
	QDomDocument doc;
	if( !doc.setContent(body) )
	{
		addtoLog("Control : update_contacts_list :  not XML file");
		return false;
	}
	QDomElement root = doc.documentElement();
	if( root.isNull() || (root.tagName() != "results") )
	{
		addtoLog(
				"Control : update_contacts_list : wrong format (no result element)");
		return false;
	}
	QDomElement elem = root.firstChildElement("contacts");
	if( elem.isNull() )
	{
		addtoLog(
				"Control : update_contacts_list : wrong format (no contacts elem)");
		return false;
	}
	//get teams info
	elem = elem.firstChildElement("buddy-lists");

	if( elem.isNull() )
	{
		addtoLog(
				"Control : update_contacts_list : wrong format (no buddy-lists elem)");
		return false;
	}
	QDomNodeList nodelist = elem.elementsByTagName("buddy-list");
	bool ok;
	int tid;
	addtoLog(tr("Control : update_contacts_list :  there are %1 teams").arg(
			nodelist.count()));
	for( int i = 0; i < nodelist.count(); i++ )
	{
		if( !nodelist.item(i).isNull() && nodelist.item(i).isElement() )
		{
			elem = nodelist.item(i).toElement();
			if( elem.hasAttribute("id") && elem.hasAttribute("name") )
			{
				tid = elem.attribute("id").toInt(&ok);
				if( ok )
				{
					groupName[tid] = elem.attribute("name");
					addtoLog(
							tr(
									"Control : update_contacts_list : got team %1 : %2") .arg(
									tid).arg(elem.attribute("name")));
				} else
				{
					addtoLog(
							"Control : update_contacts_list : team id is not int");
					return false;
				}

			} else
			{
				addtoLog("Control : update_contacts_list : invalid team info");
				return false;
			}
		} else
		{
			addtoLog("Control : update_contacts_list :  team info format error");
			return false;
		}

	}
	// get contact info
	elem = root.firstChildElement("contacts").firstChildElement("buddies");

	if( elem.isNull() )
	{
		addtoLog(
				"Control : update_contacts_list : wrong format (no buddies elem)");
		return false;
	}
	nodelist = elem.elementsByTagName("buddy");
	QString uri;
	Contact * contact;
	contactsHash.clear();
	addtoLog(tr("Control : update_contacts_list :  there are %1 contacts").arg(
			nodelist.count()));
	for( int i = 0; i < nodelist.count(); i++ )
	{
		if( !nodelist.item(i).isNull() && nodelist.item(i).isElement() )
		{
			elem = nodelist.item(i).toElement();
			if( elem.hasAttribute("uri") && elem.hasAttribute("buddy-lists") )
			{
				uri = elem.attribute("uri");
				if( !Utilities::isValidUri(uri) ) break;
				contact = new Contact(this);
				//				connect(contact, SIGNAL(contactWantToUpdateInfo(const QString &)),
				//				this, SLOT(handle_contact_update_request(const QString &)));
				connect(contact, SIGNAL(send_word( const QString & )), this,
						SLOT(handle_contact_send_word(const QString &)));

				contactsHash[uri] = contact;
				contactsHash[uri]->setUserInfo(userinfo);
				contactsHash[uri]->setCallId(50 + i);

				if( elem.attribute("buddy-lists") == "" )
				{
					tid = 0; //new contact
				} else
				{
					tid = elem.attribute("buddy-lists").toInt(&ok);
				}
				if( ok )
				{
					contact->init(uri, tid);
					addtoLog(
							tr(
									"Control : update_contacts_list : add contacts %1").arg(
									contact->getSid()));
				} else
				{
					addtoLog(
							"Control : update_contacts_list : team id is not int");
					return false;
				}
			} else
			{
				addtoLog(
						"Control : update_contacts_list : invalid contact info");
				return false;
			}
		}

	}
	return true;
}

/*************************************************
 * get contacts info
 *************************************************/
void Control::handle_contact_update_request( const QString &uri )
{
	get_contacts_info(uri);//"" means all
}

void Control::get_contacts_info( const QString &uri )
{
	/*
	 S fetion.com.cn SIP-C/2.0
	 F: 678345136
	 I: 14
	 Q: 1 S
	 N: GetContactsInfo
	 L: 314

	 <args><contacts attributes="provisioning;impresa;mobile-no;nickname;name;gender;portrait-crc;ivr-enabled" extended-attributes="score-level">
	 <contact uri="sip:585195198@fetion.com.cn;p=5005" />
	 <contact uri="sip:756444636@fetion.com.cn;p=1414" />
	 <contact uri="sip:758936988@fetion.com.cn;p=1082" />
	 </contacts></args>*/
	QString body =
			"<args><contacts attributes=\"provisioning;impresa;mobile-no;nickname;name;"
				"gender;portrait-crc;ivr-enabled\" extended-attributes=\"score-level\">";
	if( uri.isEmpty() )
	{
		//get all contacts info
		Contact * contact;
		foreach(contact, contactsHash)
			{
				body += "<contact uri=\"" + contact->getUri() + "\" />";
			}

	} else
	{
		//get specific contact info
		body += "<contact uri=\"" + uri + "\" />";
	}
	body += "</contacts></args>";
	QString sword = tr("S fetion.com.cn SIP-C/2.0\r\n"
		"F: %1\r\n"
		"I: %2 \r\n"
		"Q: 1 S\r\n"
		"N: GetContactsInfo\r\n"
		"L: %3\r\n"
		"\r\n").arg(userinfo->getSid()).arg(SipWord::IdGetContactsInfo).arg(
			body.toLocal8Bit().length()) + body;
	siptalk->send_word(sword);
	siptalk->flush();
	//	qDebug() << sword;
}

void Control::handle_sip_contacts_info( SipWord *word )
{
	QDomDocument doc;
	QDomElement root;
	QDomElement elem;
	QDomNode node;
	if( word->getStatusCode() == SipWord::CodeOk )
	{/*SIP-C/2.0 200 OK
	 I: 14
	 Q: 1 S
	 L: 127

	 <results><contacts><contact uri="sip:585195198@fetion.com.cn;p=5005"><extended score-level="3"/></contact></contacts></results>
	 */

		if( !doc.setContent(word->getBody()) )
		{
			addtoLog("Control : handle_sip_contacts_info :  not XML file");
			return;
		}
		root = doc.documentElement();
		elem = root.firstChildElement("contacts");
		elem = elem.firstChildElement("contact");

		addtoLog(
				"Control : handle_sip_contacts_info : server will return contact info");
	} else if( (word->getType() == SipWord::TypeBn) && (word->getEvent()
			== "contact") )
	{
		/* BN 678345136 SIP-C/2.0
		 N: contact
		 Q: 2 BN
		 I: 14
		 L: 347

		 <events><event type="ServiceResult"><results><contacts>
		 <contact uri="sip:585195198@fetion.com.cn;p=5005" status-code="200">
		 <personal version="20" nickname="........." gender="2" impresa=".................."
		 ivr-enabled="1" portrait-crc="2074595345" provisioning="1" mobile-no="13910423555" name=""/>
		 </contact>
		 <contact.......</contacts>
		 </contacts></results></event></events>

		 BN 678345136 SIP-C/2.0
		 N: contact
		 Q: 3 BN
		 I: 14
		 L: 267

		 <events><event type="ServiceResult"><results><contacts>
		 <contact uri="sip:756444636@fetion.com.cn;p=1414"><extended score-level="1"/></contact>
		 <contact uri="sip:758936988@fetion.com.cn;p=1082"><extended score-level="2"/></contact>
		 </contacts></results></event></events>
		 */

		if( !doc.setContent(word->getBody()) )
		{
			addtoLog("Control : handle_sip_contacts_info :  not XML file");
			return;
		}
		root = doc.documentElement();
		if( root.isNull() || (root.tagName() != "events") )
		{
			addtoLog(
					"Control : handle_sip_contacts_info : wrong format (no events element)");
			return;
		}

		elem = root.firstChildElement("event") .firstChildElement("results");
		elem = elem.firstChildElement("contacts");
		//		int count = elem.elementsByTagName("contact").count();
		elem = elem.firstChildElement("contact");
	} else
	{
		addtoLog(
				"Control : handle_sip_contacts_info : server return strange word");
		return;
	}
	//		for( int i = 0; i < count; i++ )
	int i = 0;
	while( (!elem.isNull()) && (i < 300) )
	{
		i++;
		if( elem.hasAttribute("uri") && !elem.attribute("uri").isEmpty() )
		{
			QString tmp = elem.attribute("uri");
			if( contactsHash.contains(tmp) )
			{
				contactsHash[tmp]->updateInfo(elem);
				addtoLog(
						"Control : handle_sip_contacts_info : got contact info word for "
								+ contactsHash[tmp]->getSid());
			} else
			{
				addtoLog(
						"Control : handle_sip_contacts_info : no such uri in hash");
			}
		} else
		{
			addtoLog(
					"Control : handle_sip_contacts_info : wrong format (no uri attribute)");
		}
		elem = elem.nextSiblingElement("contact");
	}
	return;

	addtoLog("Control : handle_sip_contacts_info : server return strange word");

}

/*************************************************
 * sub and process presence
 *************************************************/
void Control::sub_contacts_precense()
{
	/*	SUB fetion.com.cn SIP-C/2.0
	 F: 678345136
	 I: 13
	 Q: 1 SUB
	 N: presence
	 L: 534

	 <args><subscription><contacts>
	 <contact uri="sip:585195198@fetion.com.cn;p=5005" />
	 <contact uri="sip:756444636@fetion.com.cn;p=1414" />
	 <contact uri="sip:758936988@fetion.com.cn;p=1082" />
	 </contacts><presence><basic attributes="all" />
	 <personal attributes="all" />
	 <extended types="sms;location;listening;ring-back-tone" />
	 </presence></subscription><subscription><contacts>
	 <contact uri="sip:678345136@fetion.com.cn;p=5017" /></contacts>
	 <presence><extended types="sms;location;listening;ring-back-tone" /></presence></subscription></args>
	 */
	QString body = "<args><subscription><contacts>";
	Contact * contact;
	foreach( contact, contactsHash )
		{
			//get all contacts info
			body += "<contact uri=\"" + contact->getUri() + "\" />";
		}
	body += "</contacts><presence><basic attributes=\"all\" />"
		"<personal attributes=\"all\" />"
		"<extended types=\"sms;location;listening;ring-back-tone\" />"
		"</presence></subscription><subscription><contacts>";
	body += "<contact uri=\"" + userinfo->getUri() + "\" />";
	body += "</contacts>"
		"<presence><extended types=\"sms;location;listening;ring-back-tone\" />"
		"</presence></subscription></args>";

	siptalk->send_word(tr("SUB fetion.com.cn SIP-C/2.0\r\n"
		"F: %1\r\n"
		"I: %2 \r\n"
		"Q: 1 SUB\r\n"
		"N: presence\r\n"
		"L: %3\r\n"
		"\r\n").arg(userinfo->getSid()).arg(SipWord::IdSubPresence).arg(
			body.length()) + body);
	siptalk->flush();

}

void Control::handle_sip_contacts_precense( SipWord * word )
{

	if( word->getStatusCode() == SipWord::CodeOk )
	{
		addtoLog("Control : handle_sip_contacts_precense : sub ok");
	}
	/*BN 678345136 SIP-C/2.0
	 Q: 2 BN
	 N: presence
	 X: 25199
	 I: 13
	 L: 769

	 <events>
	 <event type="PresenceChanged">
	 <presence uri="sip:756444636@fetion.com.cn;p=1414">
	 <basic device-id="" value="0" desc="" device-type="" device-caps="" />
	 <personal nickname="......" impresa=":qw" portrait-crc="598224859" />
	 <extended type="sms">0.0:0:0</extended>
	 <extended type="listening"></extended>
	 <extended type="location"><location city="" distance="" /></extended>
	 </presence>
	 <presence uri="sip:758936988@fetion.com.cn;p=1082">
	 <basic device-id="" value="0" desc="" device-type="" device-caps="" />
	 <personal nickname="......" impresa="....................." portrait-crc="-1449886332" />
	 <extended type="sms">0.0:0:0</extended>
	 <extended type="listening"></extended>
	 <extended type="location"><location city="" distance="" /></extended>
	 </presence>
	 </event>
	 </events>*/
	if( (word->getType() == SipWord::TypeBn)
			&& (word->getEvent() == "presence") )
	{
		QDomDocument doc;
		if( !doc.setContent(word->getBody()) )
		{
			addtoLog("Control : handle_sip_contacts_precense :  not XML file");
			return;
		}
		QDomElement root = doc.documentElement();
		if( root.isNull() || (root.tagName() != "events") )
		{
			addtoLog(
					"Control : handle_sip_contacts_precense : wrong format (no events element)");
			return;
		}
		QDomElement elem;
		QDomNode node;
		elem = root.firstChildElement("event");
		if( elem.attribute("type") != "PresenceChanged" )
		{
			addtoLog(
					"Control : handle_sip_contacts_precense : wrong format (not PresenceChanged event)");
			return;
		}
		elem = elem.firstChildElement("presence");

		//		for( int i = 0; i < count; i++ )
		int i = 0;
		while( (!elem.isNull()) && (i < 300) )
		{
			i++;
			if( elem.hasAttribute("uri") && !elem.attribute("uri").isEmpty() )
			{
				QString tmp = elem.attribute("uri");
				if( contactsHash.contains(tmp) )
				{
					contactsHash[tmp]->updatePresence(elem);
					addtoLog(
							"Control : handle_sip_contacts_precense : got contact info word for "
									+ contactsHash[tmp]->getSid());
				} else
				{
					addtoLog(
							"Control : handle_sip_contacts_precense : no such uri in hash");
				}
			} else
			{
				addtoLog(
						"Control : handle_sip_contacts_precense : wrong format (no uri attribute)");
			}
			elem = elem.nextSiblingElement("presence");
		}
		return;
	} else
	{
		addtoLog(
				"Control : handle_sip_contacts_precense : server return strange word");
	}

}

/*************************************************
 * keep alive
 *************************************************/
void Control::keep_alive()
{
	//change_presence("");
	addtoLog(tr("Control: keep-alive"));
	cseq_reg++;
	siptalk->send_word(tr("R fetion.com.cn SIP-C/2.0\r\n"
		"F: %1 \r\\n"
		"I: %2 \r\n"
		"Q: %3 R\r\n\r\n").arg(userinfo->getSid()).arg(SipWord::IdRegistry).arg(cseq_reg));
	siptalk->flush();
}

/*************************************************
 * log out
 *************************************************/
void Control::tologout()
{
	siptalk->disconnectFromHost();

	//to send logout word
	return;
}

/*************************************************
 * contact specific msg, like Invite, Ack, Bye, Message, Cancel...
 *************************************************/
void Control::handle_sip_contact_specific_precense( SipWord * word )
{
	if( !word->getFrom().isEmpty() && Utilities::isValidUri(word->getFrom()) )
	{
		if( contactsHash.contains(word->getFrom()) )
		{
			contactsHash[word->getFrom()]->handle_word(word);
			return;
		} else
		{
			// no known contact
		}
	} else if( !word->getTo().isEmpty() && Utilities::isValidUri(word->getTo()) )
	{
		if( contactsHash.contains(word->getTo()) )
		{
			contactsHash[word->getTo()]->handle_word(word);
			return;
		} else
		{
			// no known contact
		}
	}
	//strange
}
/*************************************************
 * send service
 *************************************************/

void Control::handle_contact_send_word( const QString &str )
{
	siptalk->send_word(str);
}

/*************************************************
 * others
 *************************************************/
void Control::handle_userinfo_ssiStatusChanged( UserInfo::ssiStatus status )
{
	switch( status )
	{
		case UserInfo::loginToSSiPortal:
			setLoginStatus(Control::loginToSSiPortal);
			break;
		case UserInfo::loginToSSiPortalDone:
			setLoginStatus(Control::loginToSSiPortalDone);
			if( !userinfo->getProxyHost().isEmpty() )
			{
				loginToFetionProxy();
			} else
			{
				userinfo ->requestProxyHost();
				setLoginStatus(Control::getProxyHost);
			}
			break;
		case UserInfo::loginToSSiPortalError:
			setLoginStatus(Control::loginToSSiPortalError);
			break;
		case UserInfo::gettingProxyHost:
			setLoginStatus(Control::getProxyHost);
			break;
		case UserInfo::getProxyHostDone:
			setLoginStatus(Control::getProxyHostDone);
			loginToFetionProxy();
			break;
		case UserInfo::getProxyHostError:
			setLoginStatus(Control::getProxyHostError);
			break;

	}
}

bool Control::hasLogin()
{
	if( loginstatus == Control::registerToProxyDone )
	{
		return true;
	}
	return false;
}

void Control::setLoginStatus( Control::loginStatus status )
{
	this->loginstatus = status;
	loginStatusChanged(status);
}

QHash<QString, Contact *> &
Control::getContactsHash()
{
	return contactsHash;
}

QHash<int, QString> &
Control::getGroupsHash()
{
	return groupName;

}

void Control::test()
{
	//	update_contacts_list(
	//			"	 <results><contacts version=\"13\">\n"
	//				"	 <buddy-lists>\n"
	//				"	 <buddy-list id=\"1\" name=\"jiaren\" />\n"
	//				"	 <buddy-list id=\"2\" name=\"chengdu\" />\n"
	//				"	 </buddy-lists>\n"
	//				"	 <buddies>\n"
	//				"	 <buddy uri=\"sip:585195198@fetion.com.cn;p=5005\" local-name=\"\" buddy-lists=\"2\" relation-status=\"1\" online-notify=\"1\" />\n"
	//				"	 <buddy uri=\"sip:756444636@fetion.com.cn;p=1414\" local-name=\"\" buddy-lists=\"2\" relation-status=\"1\" online-notify=\"1\" />\n"
	//				"	 <buddy uri=\"sip:758936988@fetion.com.cn;p=1082\" local-name=\"\" buddy-lists=\"1\" relation-status=\"1\" online-notify=\"0\" />\n"
	//				"	 </buddies>\n"
	//				"	 <mobile-buddies />\n"
	//				"	 <chat-friends />\n"
	//				"	 </contacts></results>");
	//	contactsHash["sip:585195198@fetion.com.cn;p=5005"]->setPresence(400);
	//	contactsHash["sip:585195198@fetion.com.cn;p=5005"]->updateShow();

}
