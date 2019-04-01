#ifndef NTCPSERVER_H
#define NTCPSERVER_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>

class NTcpServer : public QTcpServer
{
	Q_OBJECT

public:
	NTcpServer(QObject *parent,int nPort);
	~NTcpServer();

	void DoListen();
	void DoClose();
	void AppendClient(QTcpSocket* session);
	QTcpSocket* Session(int index);
	int SessionIndex(QTcpSocket* session);
	void RemoveSession(QTcpSocket* session);
	void SetChannel(QString channel,QTcpSocket* session);
	void WriteByChannel(QString channel,QByteArray msg);
private:
	int m_nPort;
	QVector<QTcpSocket*> m_sessions; //在线客户端列表
	QMap<QTcpSocket*,QString> m_channleMap;
};

#endif // NTCPSERVER_H
