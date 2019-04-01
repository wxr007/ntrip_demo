#ifndef NTRIP_QT_H
#define NTRIP_QT_H

#include <QtGui/QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QMessageBox>
#include "ui_ntrip_qt.h"

#include "ntcpclient.h"
#include "ntcpserver.h"

class ntrip_qt : public QMainWindow
{
	Q_OBJECT

public:
	ntrip_qt(QWidget *parent = 0, Qt::WFlags flags = 0);
	~ntrip_qt();

private:
	Ui::ntrip_qtClass ui;
	
	NTcpServer* m_NServer;
	NTcpClient* m_Forward;
	QMap<QString,NTcpClient*> m_Forwards;
	NTcpClient* m_NClient;
private slots:
	void OnClientConnectClicked();
	void OnServerListenClicked();
	void OnClientSendMsgClicked();
	void OnServerSendMsgClicked();
	void OnCloseClientClicked();
	void OnCloseServerClicked();
	void OnForwardChecked(int nState);
	//Á¬½Ó
	void OnAcceptConnect();
	void OnClientConnected();
	void OnClientDisconnect();
	void OnServerDisconnect();
	void OnClientReceiveData();
	void OnServerReceiveData();
	//
	void OnForwardConnectClicked();
	void OnForwardCloseClicked();
	void OnForwardConnected();
	void OnForwardReceiveData();
	void OnForwardDisconnect();
};

#endif // NTRIP_QT_H
