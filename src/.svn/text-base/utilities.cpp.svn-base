#include "utilities.h"
#include <QCryptographicHash>
Utilities::Utilities()
{
}

bool Utilities::isValidUri( const QString &str )
{
	if( str.startsWith("sip:") )
	{
		return true;
	} else
	{
		return false;
	}
}
bool Utilities::isValidSid( const QString &str )
{
	return true;
}
bool Utilities::isValidMobNo( const QString &str )
{
	if( (str.length() == 11) && (str.startsWith('1')) )
	{
		return true;
	}
	return false;
}

QString Utilities::calculateDigest( const QString &nonce, const QString &uri,
									const QString &sid, const QString &passwd,
									const QString &cnonce,
									const QString &domain )
{
	//	key = MD5 (SID + “:” + Domain + “:” + Password)
	//	H1 = MD5ToHex (key + “:” + n-value + “:” + cn-value)
	//	H2 = MD5ToHex (“REGISTER” + “:” + SID)
	//	Response = MD5ToHex (H1 + “:” + n-value + “:” + H2)

	QByteArray tmp, key, h1, h2, response, sid_byte = sid.toAscii(),
			nonce_byte = nonce.toAscii(), cnonce_byte = cnonce.toAscii(),
			passwd_byte = passwd.toAscii();

	tmp.clear();
	tmp.append(sid_byte + ':' + domain + ':' + passwd_byte);
	key = QCryptographicHash::hash(tmp, QCryptographicHash::Md5);

	tmp.clear();
	tmp = key + ':' + nonce_byte + ':' + cnonce_byte;
	h1 = QCryptographicHash::hash(tmp, QCryptographicHash::Md5).toHex().toUpper();

	tmp.clear();
	tmp.append("REGISTER:").append(sid_byte);
	h2 = QCryptographicHash::hash(tmp, QCryptographicHash::Md5).toHex().toUpper();

	tmp.clear();
	tmp = h1 + ':' + nonce_byte + ':' + h2;
	response = QCryptographicHash::hash(tmp, QCryptographicHash::Md5).toHex().toUpper();

	return response;
}

QString Utilities::getSidFromUri( const QString &uri )
{
	//sip:678345136@fetion.com.cn;p=5017
	return uri.section(":", 1, 1).section("@", 0, 0);
}
