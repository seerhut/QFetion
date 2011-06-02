/*
 * sippack.cpp
 *
 *  Created on: 2008-12-2
 *      Author: seerhut
 */
#include <QDebug>
#include <QStringList>
#include "sipword.h"

SipWord::SipWord( QObject * parent ) :
	QObject(parent)
{
	init();
}

void SipWord::init()
{
	domain.clear();
	protocol.clear();
	firstLine.clear();
	body.clear();
	headint.clear();
	headstring.clear();
	status = CodeUnknown;
	type = TypeUnknown;
}

QString &SipWord::getFirstLine()
{
	return firstLine;
}

QString &SipWord::getBody()
{
	return body;

}

void SipWord::setBody( const QString &str )
{

	body = QString::fromUtf8(str.toLatin1().data());
	qDebug() << "body :" << body;
}
//
//QString &SipWord::getAuth(){
//	return headstring["A"];
//}

QString &SipWord::getWWWAuth()
{
	return headstring["W"];
}

QString &SipWord::getEvent()
{
	return headstring["N"];
}

QString &SipWord::getFrom()
{
	return headstring["F"];
}

QString &SipWord::getTo()
{
	return headstring["T"];
}

int SipWord::getStatusCode()
{
	return status;
}
int SipWord::getType()
{
	return type;
}
int SipWord::getCallId()
{
	if( !headint.contains("I") )
	{
		return 0;
	}
	return headint["I"];
}
int SipWord::getCSeq()
{
	if( !headint.contains("Q") )
	{
		return 0;
	}
	return headint["Q"];
}

int SipWord::getLength()
{
	if( !headint.contains("L") )
	{
		return 0;
	}
	return headint["L"];
}

//int SipWord::checkWord(const QString&word)
//{
//	QString tmp, head, body;
//	bool ok;
//	head = word.section("\r\n\r\n", 0, 0);
//	body = word.section("\r\n\r\n", 1, -1);
//	if (body.isEmpty())
//	{
//		return SipWord::WordValidBody;
//	}
//	foreach(tmp , head.split("\r\n"))
//		{
//			if (tmp.startsWith("L: "))
//			{
//				tmp = tmp.section(" ", 1, 1);
//				int length = tmp.toInt(&ok);
//				if (!ok)
//				{
//					qDebug() << "SipWord:  invalid length";
//					return SipWord::WordNoLengthInfo;
//				}
//				int actual_size = body.length();
//				if (actual_size == length)
//					return SipWord::WordValidBody;
//				if (actual_size < length)
//				{
//					qDebug() << "actual size :" << actual_size;
//					qDebug() << "labeled size :" << length;
//					return SipWord::WordIncompleteBody;
//				}
//				if (actual_size > length)
//				{
//					qDebug() << "actual size :" << actual_size;
//					qDebug() << "labeled size :" << length;
//					return SipWord::WordBodyOutOfSize;
//				}
//			}
//		}
//	qDebug() << "SipWord:  cannot find length";
//	return SipWord::WordNoLengthInfo;
//}
void SipWord::setFirstLine( const QString &line )
{
	firstLine = line;
	if( !line.startsWith("SIP-C/2.0") )
	{
		status = SipWord::CodeNone;
		return;
	}

	QString tmp = line.section(" ", 1, 1);
	if( tmp == "401" )
	{
		status = SipWord::CodeUnauthorized;
		qDebug() << "401";
		return;
	}
	if( tmp == "200" )
	{
		status = SipWord::CodeOk;
		qDebug() << "200";
		return;
	}
	if( tmp == "406" )
	{
		status = SipWord::CodeNotAcceptable;
		return;
	}
	if( tmp == "280" )
	{
		status = SipWord::CodeSendSMSOk;
		return;
	}

	status = SipWord::CodeUnknown;
}

void SipWord::addHeadLine( const QString &line )
{
	/*F: 678345136
	 I: 12
	 Q: 1 S
	 N: GetContactPermission
	 L: 50
	 X: 25199
	 T: sip:PG5709625@fetion.com.cn;p=1414
	 K: text/html-fragment
	 K: multiparty
	 K: nudge
	 W: Digest algorithm="MD5-sess;SHA1-sess",nonce="115D841444272AA115CEFF255BD78468"
	 A: Digest algorithm="SHA1-sess",response="466E9193E6BD7C14D461C62DF040752C",cnonce="31AA711B2F6E4FF1182EBFF32CB08171",salt="5FE81000"
	 */

	if( (line[1] != ':') || (line[2] != ' ') )
	{
		addtoLog("SipWord : addHeadLine : invalid head line");
	}
	QString h = line.section(":", 0, 0);
	if( (h == "I") || (h == "L") || (h == "Q") )
	{
		headint[h] = line.trimmed().section(" ", 1, 1).toInt();
		if( h == "Q" )
		{//get type
			QString tmp = line.trimmed().section(" ", 2, 2);
			if( tmp == "R" )
			{
				type = SipWord::TypeReg;
				return;
			}
			if( tmp == "SUB" )
			{
				type = SipWord::TypeSubscribe;
				return;
			}
			if( tmp == "M" )
			{
				type = SipWord::TypeMessage;
				return;
			}
			if( tmp == "I" )
			{
				type = SipWord::TypeInvite;
				return;
			}
			if( tmp == "B" )
			{
				type = SipWord::TypeBye;
				return;
			}
			if( tmp == "BN" )
			{
				type = SipWord::TypeBn;
				return;
			}
			if( tmp == "S" )
			{
				type = SipWord::TypeService;
				return;
			}
			if( tmp == "A" )
			{
				type = SipWord::TypeAck;
				return;
			}
			if( tmp == "C" )
			{
				type = SipWord::TypeCancel;
				return;
			}
			if( tmp == "IN" )
			{
				type = SipWord::TypeInfo;
				return;
			}
			if( tmp == "N" )
			{
				type = SipWord::TypeNotify;
				return;
			}
			if( tmp == "O" )
			{
				type = SipWord::TypeOptions;
				return;
			}
			type = SipWord::TypeUnknown;
		}
		return;
	}
	if( (h == "N") || (h == "X") || (h == "XI") || (h == "T") || (h == "K")
			|| (h == "F") || (h == "W") )
	{
		headstring[h] = line.trimmed().section(" ", 1, -1);
	}

	return;
}

