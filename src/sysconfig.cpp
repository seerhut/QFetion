#include "sysconfig.h"
#include <QDebug>

SysConfig::SysConfig( QObject * parent ) :
	QObject(parent)
{
	//	qDebug() << "SysConfig Class Loading... ";
	init();
	qDebug() << "SysConfig Loaded";
}

void SysConfig::init()
{
	ssihost = "";
	ssipath = "";
	hello_str = "";
	ssiport = 443;
}

void SysConfig::initFromConfig()
{
	//tmp value
	init();
	settings.beginGroup("sysconfig");
	ssiport = settings.value("ssiport", 443).toInt();
	ssihost = settings.value("ssihost", "nav.fetion.com.cn").toString();
	px_port = settings.value("px_port", 0).toInt();
	px_host = settings.value("px_host", "").toString();
	px_user = settings.value("px_user", "").toString();
	px_pass = settings.value("px_pass", "").toString();
	px_type = settings.value("px_type", 0).toInt();
	ssipath = settings.value("ssipath",
			"/ssiportal/SSIAppSignIn.aspx?mobileno=%1&pwd=%2").toString();

	hello_str=tr("<args><device type=\"PC\" version=\"293423607\" client-version=\"3.4.1280\" />"
						"<caps value=\"simple-im;im-session;temp-group;personal-group\" />"
						"<events value=\"contact;permission;system-message;personal-group\" />"
						"<oem-tag value=\"cmcc.sjb0.inet.0024\" /><user-info attributes=\"all\" />"
						"<presence><basic value=\"400\" desc=\"\" /></presence></args>");
	settings.endGroup();

	qDebug() << "SysConfig inited from Config file";
}
void SysConfig::updateFromInfoServer()
{

}
void SysConfig::saveToConfig()
{
	settings.beginGroup("sysconfig");
	settings.setValue("ssiport", ssiport);
	settings.setValue("ssihost", ssihost);
	settings.setValue("ssipath", ssipath);
	settings.setValue("px_port", px_port);
	settings.setValue("px_host", px_host);
	settings.setValue("px_user", px_user);
	settings.setValue("px_pass", px_pass);
	settings.setValue("px_type", px_type);
	settings.setValue("hello_str", hello_str);

	settings.endGroup();
	settings.sync();
	qDebug() << "SysConfig saved";
}

void SysConfig::setPxInfo(int type, QString host, int port, QString user, QString pass){
	px_type = type;
	px_host = host;
	px_port = port;
	px_user = user;
	px_pass = pass;
}

const QString SysConfig::getDefaultAcount()
{
	return settings.value("default_mob", "").toString();
}

//bool SysConfig::hasConfigFor( const QString &mob )
//{
//	//	if( mob == "13551325903" )
//	//	{
//	//		return true;
//	//	}
//	return false;
//}

//bool SysConfig::hasDefaultAcount()
//{
//	return false;
//}

const QString &SysConfig::getSsiPortalHost()
{
	return ssihost;
}
const int SysConfig::getSsiPortalPort()
{
	return ssiport;
}
const QString &SysConfig::getSsiPortalPath()
{
	//include "%1 %2"from mob_no and passwd
	return ssipath;
}

const QString &SysConfig::getHelloStr()
{
	return hello_str;
}
//
//void SysConfig::setSsiPortalHost( const QString &host )
//{
//	ssihost = host;
//}
//void SysConfig::setSsiPortalPort( int port )
//{
//	ssiport = port;
//}
//void SysConfig::setSsiPortalPath( const QString &path )
//{
//	ssipath = path;
//}
//
//void SysConfig::setHelloStr( const QString &hello )
//{
//	hello_str = hello;
//}
