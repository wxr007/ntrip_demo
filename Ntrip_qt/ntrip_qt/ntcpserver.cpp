#include "ntcpserver.h"

NTcpServer::NTcpServer(QObject *parent,int nPort)
	: QTcpServer(parent)
	,m_nPort(nPort)
{
	m_sessions.clear();
}

NTcpServer::~NTcpServer()
{

}

void NTcpServer::DoListen()
{
	listen(QHostAddress::Any,m_nPort);
}

void NTcpServer::DoClose()
{
	for(int i=0;i<m_sessions.count();i++)
	{
		m_sessions.at(i)->close();//关闭所有客户端
	}
	close();
}

void NTcpServer::AppendClient(QTcpSocket* client)
{
	m_sessions.append(client);
}

QTcpSocket* NTcpServer::Session(int index)
{
	return m_sessions.at(index);
}

int NTcpServer::SessionIndex(QTcpSocket* session)
{
	return m_sessions.indexOf(session);
}

void NTcpServer::RemoveSession(QTcpSocket* session)
{
	int index = m_sessions.indexOf(session);
	m_sessions.remove(index);
	m_channleMap.remove(session);
}

void NTcpServer::SetChannel(QString channel,QTcpSocket* session)
{
	m_channleMap.insert(session,channel);
}

void NTcpServer::WriteByChannel(QString channel,QByteArray msg)
{
	for(auto it = m_channleMap.begin();it != m_channleMap.end();it++)
	{
		if (it.value() == channel)
		{
			it.key()->write(msg);
		}
	}
}
