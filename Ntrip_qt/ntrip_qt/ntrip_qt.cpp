#include "ntrip_qt.h"

ntrip_qt::ntrip_qt(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	m_Forwards.clear();

	connect(ui.clientConnect,SIGNAL(clicked()),this,SLOT(OnClientConnectClicked()));
	connect(ui.serverListen,SIGNAL(clicked()),this,SLOT(OnServerListenClicked()));

	connect(ui.clientSend,SIGNAL(clicked()),this,SLOT(OnClientSendMsgClicked()));
	connect(ui.severSend,SIGNAL(clicked()),this,SLOT(OnServerSendMsgClicked()));

	connect(ui.clientDisconnect,SIGNAL(clicked()),this,SLOT(OnCloseClientClicked()));
	connect(ui.serverClose,SIGNAL(clicked()),this,SLOT(OnCloseServerClicked()));

	connect(ui.ForwardCheck,SIGNAL(stateChanged(int)),this,SLOT(OnForwardChecked(int)));
}

ntrip_qt::~ntrip_qt()
{

}

void ntrip_qt::OnClientConnectClicked()
{
	if(ui.clientIpEdit->text().isEmpty() || ui.clientPortEdit->text().isEmpty())
	{
		QMessageBox::warning(this,"Tips","Please input ip and port");
		return;
	}

	m_NClient = new NTcpClient(this,ui.clientIpEdit->text(),ui.clientPortEdit->text().toInt());
	connect(m_NClient,SIGNAL(connected()),this,SLOT(OnClientConnected()));//检测链接成功信号
	connect(m_NClient,SIGNAL(readyRead()),this,SLOT(OnClientReceiveData()));//关联接收数据信号
	connect(m_NClient,SIGNAL(disconnected()),this,SLOT(OnClientDisconnect()));//关联掉线信号
	m_NClient->DoConnect();

	ui.TabPage->setTabEnabled(1, false);
}

void ntrip_qt::OnServerListenClicked()
{
	if(ui.serverPortEdit->text().isEmpty())
	{
		QMessageBox::warning(this,"Tips","Please input port");
		return;
	}

	m_NServer = new NTcpServer(this,ui.serverPortEdit->text().toInt());
	connect(m_NServer,SIGNAL(newConnection()),this,SLOT(OnAcceptConnect()));//关联新客户端链接信号
	m_NServer->DoListen();//启动服务器监听

	ui.serverListen->setEnabled(false);
	ui.severSend->setEnabled(true);
	ui.TabPage->setTabEnabled(0, false);
}

void ntrip_qt::OnClientSendMsgClicked()
{
	QString ChannelName = ui.channelSelect->currentText();
	QString NtripData = QString("GET %1 HTTP/1.1\r\nUser-Agent: NTRIP JS Client/0.2\r\nAuthorization: Basic eXlkZ2lzOnl1ZGFuMjAwOA==\n\n").arg(ChannelName);
	m_NClient->write(NtripData.toUtf8());
	ui.clientSendText->appendPlainText(NtripData);
}

void ntrip_qt::OnServerSendMsgClicked()
{
	int nRow = ui.onlineUserList->currentRow();
	QTcpSocket* SessionSocket = m_NServer->Session(nRow);
	if (SessionSocket)
	{
		SessionSocket->write(ui.serverSendText->toPlainText().toUtf8());
	}
}

void ntrip_qt::OnCloseClientClicked()
{
	m_NClient->close();//关闭客户端
	ui.clientConnect->setEnabled(true);
	ui.clientSend->setEnabled(false);
}

void ntrip_qt::OnCloseServerClicked()
{
	m_NServer->DoClose();
	ui.serverListen->setEnabled(true);
	ui.severSend->setEnabled(false);
}

void ntrip_qt::OnForwardChecked(int nState)
{
	if (nState == 0)//正常
	{
		OnForwardCloseClicked();
		ui.severSend->show();
	}
	else if (nState == 2)//转发
	{
		OnForwardConnectClicked();
		ui.severSend->hide();
	}
}

void ntrip_qt::OnAcceptConnect()
{
	QTcpSocket* sessionSocket = m_NServer->nextPendingConnection(); //返回与客户端连接通信的套接字
	connect(sessionSocket,SIGNAL(readyRead()),this,SLOT(OnServerReceiveData()));//关联接收数据信号
	connect(sessionSocket,SIGNAL(disconnected()),this,SLOT(OnServerDisconnect()));//关联掉线信号
	m_NServer->AppendClient(sessionSocket);
	
	QString ip = sessionSocket->peerAddress().toString().remove("::ffff:");//去除客户端中多余的字符
	ui.onlineUserList->addItem(ip);
}

void ntrip_qt::OnClientConnected()
{
	ui.clientConnect->setEnabled(false);
	ui.clientSend->setEnabled(true);
}

void ntrip_qt::OnClientDisconnect()
{
	ui.clientConnect->setEnabled(true);//断开连接的时候重新启用开始按钮
	ui.clientSend->setEnabled(false);
}

void ntrip_qt::OnServerDisconnect()
{
	QTcpSocket *obj = (QTcpSocket*)sender();//获取掉线对象
	int row = m_NServer->SessionIndex(obj);//找到掉线对象的内容所在的行
	QListWidgetItem *item = ui.onlineUserList->takeItem(row);//从界面列表中去除找到的一行内容
	delete item;
	m_NServer->RemoveSession(obj);
}

void ntrip_qt::OnClientReceiveData()
{
	QTcpSocket *obj = (QTcpSocket*)sender();
	//获取发送数据端的IP
	QString ip = obj->peerAddress().toString();
	ip.remove("::ffff:");
	QString msg = obj->readAll();
	ui.clientReceiveText->appendPlainText(msg);
}

void ntrip_qt::OnServerReceiveData()
{
	QTcpSocket *obj = (QTcpSocket*)sender();
	//获取发送数据端的IP
	QString ip = obj->peerAddress().toString();
	ip.remove("::ffff:");
	QByteArray msgArray = obj->readAll();
	QString msg = msgArray;
	ui.serverReceiveText->appendPlainText(msg);

	if (ui.ForwardCheck->isChecked())
	{
		QRegExp rxlen("SF0[1-3]");
		int ret = rxlen.indexIn(msg);
		if (ret != -1)
		{
			QString channel =  rxlen.cap(0);
			auto it = m_Forwards.find(channel);
			if (it != m_Forwards.end())
			{
				it.value()->write(msgArray);
				m_NServer->SetChannel(channel,obj);
			}
		}
		//m_Forward->write(msgArray);
	}
}

void ntrip_qt::OnForwardConnectClicked()
{
	if (m_Forwards.size() == 0)
	{
		for(int i = 1;i <=3;i++)
		{
			QString channel = QString("SF0%1").arg(QString::number(i));
			NTcpClient* forward = new NTcpClient(this,ui.ForwardIpEdit->text(),ui.serverPortEdit->text().toInt());//设置服务器的 ip和端口号
			connect(forward,SIGNAL(connected()),this,SLOT(OnForwardConnected()));//检测链接成功信号
			connect(forward,SIGNAL(readyRead()),this,SLOT(OnForwardReceiveData()));//关联接收数据信号
			connect(forward,SIGNAL(disconnected()),this,SLOT(OnForwardDisconnect()));//关联掉线信号
			forward->SetChannel(channel);
			forward->DoConnect();
			m_Forwards.insert(channel,forward);
		}
	}
	else
	{
		for(auto it = m_Forwards.begin();it != m_Forwards.end();it++)
		{
			it.value()->DoConnect();
		}
	}

	//m_Forward = new NTcpClient(this,ui.ForwardIpEdit->text(),ui.serverPortEdit->text().toInt());//设置服务器的 ip和端口号
	//connect(m_Forward,SIGNAL(connected()),this,SLOT(OnForwardConnected()));//检测链接成功信号
	//connect(m_Forward,SIGNAL(readyRead()),this,SLOT(OnForwardReceiveData()));//关联接收数据信号
	//connect(m_Forward,SIGNAL(disconnected()),this,SLOT(OnForwardDisconnect()));//关联掉线信号
	//m_Forward->DoConnect();

	
}

void ntrip_qt::OnForwardCloseClicked()
{
	for(auto it = m_Forwards.begin();it != m_Forwards.end();it++)
	{
		it.value()->close();
	}
	//m_Forward->close();
}

void ntrip_qt::OnForwardConnected()
{

}

void ntrip_qt::OnForwardReceiveData()
{
	NTcpClient *obj = (NTcpClient*)sender();
	//获取发送数据端的IP
	QString ip = obj->peerAddress().toString();
	ip.remove("::ffff:");
	QByteArray msgArray = obj->readAll();
	QString msg = msgArray;
	ui.serverSendText->appendPlainText(msg);

	//int nRow = ui.onlineUserList->currentRow();
	//if(nRow < 0)
	//{
	//	return;
	//}
	//QTcpSocket* SessionSocket = m_NServer->Session(nRow);
	//if (SessionSocket)
	//{
	//	SessionSocket->write(msgArray);
	//}
	m_NServer->WriteByChannel(obj->GetChannel(),msgArray);
}

void ntrip_qt::OnForwardDisconnect()
{
	if (ui.ForwardCheck->isChecked())
	{
		NTcpClient *obj = (NTcpClient*)sender();
		//m_ForwardSocket->connectToHost(ui.ForwardIpEdit->text(),ui.serverPortEdit->text().toInt());
		obj->DoConnect();
	}
}

