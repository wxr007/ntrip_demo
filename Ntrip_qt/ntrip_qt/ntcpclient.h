#ifndef NTCPCLIENT_H
#define NTCPCLIENT_H

#include <QObject>
#include <QTcpSocket>

class NTcpClient : public QTcpSocket
{
	Q_OBJECT

public:
	NTcpClient(QObject *parent,QString strIp,int nPort);
	~NTcpClient();

	void DoConnect();
	void SetChannel(QString channel);
	QString GetChannel();
private:
	QString m_strIp;
	int m_nPort;
	QString m_strChannel;
};

#endif // NTCPCLIENT_H
