#include "userinfo.h"
#include "logintossiportal.h"
#include "utilities.h"
#include "sysconfig.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QPixmap>
class LoginToSsiPortal;

//#include "siptalk.h"

UserInfo::UserInfo( QObject * parent ) :
	QObject(parent)
{
	//	qDebug() << "UserInfo Class Loading... ";
	init();

	manager = new QNetworkAccessManager(this);

	connect(manager, SIGNAL(finished(QNetworkReply*)),
	this, SLOT(handle_getPortrait(QNetworkReply*)));

	//load loging to ssi portal service
	loginssi = new LoginToSsiPortal(this);
	connect(loginssi, SIGNAL(addtoLog(const QString &)), this, SIGNAL(addtoLog(const QString &)));
	connect(loginssi, SIGNAL(finished1(bool , const QString &, const QString &)),
	this, SLOT(handle_ssi_response(bool, const QString &, const QString &)));
	connect(loginssi, SIGNAL(got_proxyinfo(bool , const QString &, const QString &)),
	this, SLOT(handle_proxy_response(bool, const QString &, const QString &)));
	connect(loginssi, SIGNAL(got_ssic(const QString &)),
	this, SLOT(handle_got_ssic(const QString &)));
	qDebug() << "UserInfo Loaded";
}

bool UserInfo::initFromConfig( const QString &mob )
{ //load from saved config file on the computer
	init();
	mobno = mob;
	settings.beginGroup(mob);
	domain = settings.value("domain", "fetion.com.cn").toString();
	cnonce
			= settings.value("cnonce", "7C81D86A4A9B755155FA99B81AE86FAD").toString();
	proxyport = settings.value("proxyport", 8080).toInt();
	proxyhost = settings.value("proxyhost", "").toString();
	get_portrait_url = settings.value("get_portrait_url", "").toString();
	presence = settings.value("presence", online).toInt();
	savePasswd = settings.value("savepasswd", false).toBool();
	if( savePasswd )
	{
		passwd = settings.value("passwd", "").toString();
	} else
	{
		passwd = "";
	}
	uri = settings.value("uri", "").toString();
	sid = settings.value("sid", "").toString();
	complaint = settings.value("complaint", "").toString();
	nickname = settings.value("nickname", "").toString();
	settings.endGroup();

	qDebug() << "UserInfo Inited from Config file";
	return true;
}
bool UserInfo::updateInfo( const QString &body )
{/*SIP-C/2.0 200 OK
 I: 2
 Q: 1 S
 L: 703

 <results>
 <personal version="6" mobile-no="13911680859" nickname="......" impresa="" name=""
 gender="1" birth-date="1982-08-14" birthday-valid="1" nation="CN" province="bj" city="10"
 profile="" lunar-animal="11" horoscope="8" blood-type="1" occupation="" hobby=""
 portrait-crc="598224859" job-title="" home-phone="" work-phone="" other-phone=""
 personal-email="seerhut@gmail.com" work-email="" other-email="" primary-email="1" company=""
 company-website="" ivr-enabled="1" match-enabled="0" />
 <services version="6">
 <service id="Imps" name="............" desc="...........................10086..."/>
 </services>
 <config version="13" save-message="0" experience="0" save-mms="0" />
 <mobile-device />
 </results>*/
	//	addtoLog(word.section("\r\n\r\n",1,1));
	//	QString tmp = word.section("\r\n\r\n",1,1);
	//	if(tmp.startsWith("<results><personal"))
	QDomDocument doc;
	if( !doc.setContent(body) )
	{
		addtoLog("UserInfo : updateInfo :  not XML file");
		return false;
	}
	QDomElement root = doc.documentElement();
	if( root.isNull() || (root.tagName() != "results") )
	{
		addtoLog("UserInfo : updateInfo : wrong format (no result element)");
		return false;
	}
	QDomElement userElem = root.firstChildElement("personal");
	if( userElem.isNull() || (userElem.tagName() != "personal") )
	{
		addtoLog("UserInfo : updateInfo : wrong format (no personal elem)");
		return false;
	}
	if( userElem.attribute("mobile-no") != mobno )
	{
		addtoLog("UserInfo : updateInfo : different mobno!!!!");
		return false;
	}
	if( userElem.hasAttribute("nickname") && (userElem.attribute("nickname")
			!= nickname) )
	{
		nickname = userElem.attribute("nickname");
		addtoLog(tr("UserInfo : updateInfo : got diff nickname : %1").arg(
				nickname));
	}
	// God ...impresa
	if( userElem.hasAttribute("impresa") && (userElem.attribute("impresa")
			!= complaint) )
	{
		complaint = userElem.attribute("impresa");
		addtoLog(tr("UserInfo : updateInfo : got diff complaint : %1").arg(
				complaint));
	}
	if( true ) // portrait crc
	{
//		requestPortrait();
	}
	//TODO add other userinfo
	saveToConfig();
	return true;
}
bool UserInfo::saveToConfig()
{
	settings.beginGroup(mobno);
	settings.setValue("domain", domain);
	settings.setValue("cnonce", cnonce);
	settings.setValue("proxyport", proxyport);
	settings.setValue("proxyhost", proxyhost);
	settings.setValue("get_portrait_url", get_portrait_url);
	settings.setValue("presence", presence);
	if( savePasswd )
	{
		settings.setValue("passwd", passwd);
	} else
	{
		settings.setValue("passwd", "");
	}
	settings.setValue("savepasswd", savePasswd);
	settings.setValue("uri", uri);
	settings.setValue("sid", sid);
	settings.setValue("complaint", complaint);
	settings.setValue("nickname", nickname);
	settings.endGroup();
	//general...
	settings.setValue("default_mob", mobno);
	return true;
}

void UserInfo::requestUriFromSsiPortal()
{
	//	sbMsg("Login to SSI Portal...", 0);
	if( !mobno.isEmpty() && !passwd.isEmpty() )
	{
		loginssi->setAcountName(mobno);
		loginssi->setAcountPass(passwd);
		loginssi->setSysConfig(syscfg);
		ssiStatusChanged(UserInfo::loginToSSiPortal);
		loginssi->doLogin();
	} else
	{

	}
	return;

}

void UserInfo::requestProxyHost()
{
	ssiStatusChanged(UserInfo::gettingProxyHost);
	loginssi->toGetProxyHost(mobno);
}

void UserInfo::requestPortrait()
{
	QNetworkRequest request;
	///hds/getportrait.aspx?Uri=sip:678345136@fetion.com.cn;p=5017&Size=96&c=DAgHAAD...3d%3d
	QUrl url;
	url.setUrl(get_portrait_url);
	url.addQueryItem("Uri", uri);
	url.addQueryItem("Size", "96");
	url.addQueryItem("c", QString(QUrl::toPercentEncoding(ssic)));
	request.setUrl(url);
	qDebug() <<QString(QUrl::toPercentEncoding(ssic));
	qDebug() << url.toString();
	request.setRawHeader("Accept",
			"image/pjpeg;image/jpeg;image/bmp;image/x-windows-bmp;image/png;image/gif");
	manager->get(request);

}

void UserInfo::handle_getPortrait( QNetworkReply* reply )
{
	portrait = reply->readAll();
}

void UserInfo::handle_ssi_response( bool error, const QString &uri,
									const QString &errors )
{
	if( error )
	{
		ssiStatusChanged(UserInfo::loginToSSiPortalError);
		addtoLog(tr("Login to SSI portal error: %1").arg(errors));
		//		sbMsg("Login error");
	} else
	{
		this->uri = uri;
		this->sid = Utilities::getSidFromUri(uri);
		addtoLog(tr("got uri: %1").arg(this->uri));
		addtoLog(tr("got sid: %1").arg(this->sid));
		//		addtoLog(tr("Login to fetion.com.cn Proxy..."));
		//		sbMsg("Login to fetion.com.cn Proxy...");
		ssiStatusChanged(UserInfo::loginToSSiPortalDone);

	}
}

void UserInfo::handle_proxy_response( bool error, const QString &name,
										const QString &str )
{
	if( error )
	{
		ssiStatusChanged(UserInfo::getProxyHostError);
		addtoLog(tr("get proxy host error: %1").arg(str));
		//		sbMsg("Login error");
	} else
	{
		if( name == "sipc-proxy" )
		{
			this->proxyhost = str;
			addtoLog(tr("got proxy: %1").arg(this->proxyhost));
			ssiStatusChanged(UserInfo::getProxyHostDone);
			return;
		}
		if( name == "get-portrait" )
		{
			this->get_portrait_url = str;
			addtoLog(tr("got get_portrait_url: %1").arg(this->get_portrait_url));
			return;
		}
	}
}

void UserInfo::handle_got_ssic( const QString &str )
{
	ssic = str;
}

const QString &UserInfo::getSid()
{
	return sid;
}
const QString &UserInfo::getUri()
{
	return uri;
}
const QString &UserInfo::getMobNo()
{
	return mobno;
}
const QString &UserInfo::getPasswd()
{
	return passwd;
}
const QString &UserInfo::getNickname()
{
	return nickname;
}
const QString &UserInfo::getComplaint()
{
	return complaint;
}

const QString &UserInfo::getDomain()
{
	return domain;
}

const QByteArray &UserInfo::getPortrait()
{
	return portrait;
}

int UserInfo::getPresence()
{
	return presence;
}

bool UserInfo::isSavePasswd()
{
	return savePasswd;
}

const QString &UserInfo::getProxyHost()
{
	return proxyhost;
}
const int UserInfo::getProxyPort()
{
	return proxyport;
}
const QString &UserInfo::getProxyCnonce()
{
	return cnonce;
}

//void UserInfo::setSid(const QString &str)
//{
//	sid = str;
//}
//void UserInfo::setUri(const QString &str)
//{
//	uri = str;
//	sid = Utilities::getSidFromUri(uri);
//}
//void UserInfo::setMobNo(const QString &str)
//{
//	mobno = str;
//}
void UserInfo::setPasswd( const QString &str )
{
	passwd = str;
}
void UserInfo::setNickname( const QString &str )
{
	nickname = str;
}
void UserInfo::setComplaint( const QString &str )
{
	complaint = str;
}

void UserInfo::setSavePasswd( bool save )
{
	savePasswd = save;
}

void UserInfo::setDomain( const QString &str )
{
	domain = str;
}

void UserInfo::setPresence( int pre )
{
	presence = pre;
}

void UserInfo::setSysConfig( SysConfig *config )
{
	this->syscfg = config;
}
//
//void UserInfo::setSipTalk(SipTalk *config){
//	this->siptalk = config;
//}

void UserInfo::setProxyHost( const QString &host )
{
	proxyhost = host;
}
void UserInfo::setProxyPort( int port )
{
	proxyport = port;
}
void UserInfo::setProxyCnonce( const QString &cnonce )
{
	this->cnonce = cnonce;
}

void UserInfo::init()
{
	sid = "";
	uri = "";
	mobno = "";
	passwd = "";
	nickname = "";
	complaint = "";
	savePasswd = false;
	autoLogin = false;
	domain = "";
	proxyhost = "";
	cnonce = "7C81D86A4A9B755155FA99B81AE86FAD";
}

