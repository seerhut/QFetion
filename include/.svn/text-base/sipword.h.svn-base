/*
 * sipword.h
 *
 *  Created on: 2008-12-2
 *      Author: seerhut
 */
/*sip head for fetion

 */

#ifndef SIPWORD_H_
#define SIPWORD_H_

#include <QObject>
#include <QString>
#include <QHash>

class SipWord : public QObject
{
Q_OBJECT

public:
	enum statusCode
	{
		CodeUnauthorized = 401,
		CodeOk = 200,
		CodeBadRequest = 400,
		CodeNotAcceptable = 406,
		CodeServerInternalError = 500,
		CodeServerTimeOut = 504,
		CodeSendSMSOk = 280,
		CodeTry = 100,
		CodeUnknown = 0,
		CodeNone = -1

	};
	enum wordType
	{
		TypeReg = 1, //R
		TypeSubscribe, //SUB
		TypeMessage, //M
		TypeInvite,
		TypeService, //S
		TypeAck, //A
		TypeBn, //BN
		TypeBye, //B
		TypeCancel, //C
		TypeInfo, //IN
		TypeNotify, //N
		TypeOptions, //O
		TypeUnknown = 0,
		TypeNone = -1
	};

	//	enum headLine
	//	{
	//		Authorization = 1, // "A"
	//		CallID, //"I"
	//		Contact, //"M"
	//		ContentEncoding, //"E"
	//		ContentLength, //"L"
	//		ContentType, //"C"
	//		CSeq, // "Q"
	//		Date, //"D"
	//		EndPoints, // "EP"
	//		Event, // "N"
	//		Expires, //   "X"
	//		From, //"F"
	//		MessageID, //"XI"
	//		ReferredBy, // "RB"
	//		ReferTo, // "RT"
	//		Require, //"RQ"
	//		RosterManager, //"RM"
	//		Source, //"SO"
	//		Supported, // "K"
	//		To, // "T"
	//		Unsupported, //"UK"
	//		WWWAuthenticate, //"W"
	//      ???? "IN"
	//		HeadUnknown = 0,
	//		HeadNone = -1
	//	};
	enum CallId
	{
		IdRegistry = 1,
		IdGetPersonalInfo,
		IdGetContactsList,
		IdGetGroupList,
		IdGetContactsInfo,
		GetGroupInfo,
		IdGetOfflineMessages,
		IdSetPresence,
		IdSubPresence,
		IdGetScore
	// dynamic callid should be above 30
	};
	enum wordCorrectness
	{
		WordNoProtocol,
		WordNoHead,
		WordValidBody,
		WordIncompleteBody,
		WordBodyOutOfSize,
		WordNoLengthInfo
	};
	SipWord( QObject * parent );
	QObject * parent;
	void init();
	QString &getFirstLine();
	QString &getBody();
	int getStatusCode();
	int getType();
	int getCallId();
	int getCSeq();
	int getLength();
	//	QString &getAuth();
	QString &getWWWAuth();
	QString &getEvent();
	QString &getFrom();
	QString &getTo();

	void setBody( const QString & );
	//	int checkWord(const QString&);
	void parseWord( const QString & );
	void setFirstLine( const QString &line );
	void addHeadLine( const QString &line );
	signals:
	void addtoLog( const QString & );

private:
	QString domain, protocol, firstLine, body;
	statusCode status;
	wordType type;
	//	CallId callid;

	QHash<QString, int> headint;
	QHash<QString, QString> headstring;

private slots:

};

#endif /* SIPWORD_H_ */
