#include "ntcpclient.h"

NTcpClient::NTcpClient(QObject *parent,QString strIp,int nPort)
	: QTcpSocket(parent)
	,m_strIp(strIp)
	,m_nPort(nPort)
	,m_strChannel("")
{
}

NTcpClient::~NTcpClient()
{
	close();
}

void NTcpClient::DoConnect()
{
	connectToHost(m_strIp,m_nPort);//设置服务器的 ip和端口号
}

void NTcpClient::SetChannel(QString channel)
{
	m_strChannel = channel;
}

QString NTcpClient::GetChannel()
{
	return m_strChannel;
}
