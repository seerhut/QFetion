#ifndef UTILITIES_H
#define UTILITIES_H

#include <QObject>

class QCryptographicHash;

class MD5;

class Utilities : public QObject
{
public:
	Utilities();
	static bool isValidUri( const QString & );
	static bool isValidSid( const QString & );
	static bool isValidMobNo( const QString & );

	static QString calculateDigest( const QString &nonce, const QString &uri,
									const QString &sid, const QString &passwd,
									const QString &cnonce,
									const QString &domain );
	static QString getSidFromUri( const QString & );
};

#endif // UTILITIES_H
