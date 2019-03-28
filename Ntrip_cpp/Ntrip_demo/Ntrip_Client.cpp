#include "Ntrip_Client.h"
#include <iostream>

Ntrip_Client::Ntrip_Client(boost::asio::io_service& io_service,std::string strIp, int nPort, std::string strAgent, std::string strAuthorization, std::string strUri)
	:m_io(io_service)
	,m_strIp(strIp)
	,m_nPort(nPort)
	,m_strAgent(strAgent)
	,m_strAuthorization(strAuthorization)
	,m_strUri(strUri)
	,m_Sock(io_service)
{
	m_ep(ip::address::from_string(m_strIp), m_nPort);
	m_Sock.async_connect(m_ep, std::bind(&Ntrip_Client::onConnect, this, std::placeholders::_1));
}

Ntrip_Client::~Ntrip_Client()
{
}

void Ntrip_Client::onConnect(const boost::system::error_code & ec) {
	if (ec)
	{
		return;
	}
	std::cout << "Connected:" << m_pSock->remote_endpoint().address() << std::endl;
	m_Sock.async_read_some(boost::asio::buffer(m_buf, MAX_BUFF_LEN),std::bind(&Ntrip_Client::onRead, this, std::placeholders::_1));
}

void Ntrip_Client::onRead(const boost::system::error_code & ec)
{
	if (ec)
	{
		return;
	}
	std::cout << m_buf << std::endl;
	callback(m_buf, MAX_BUFF_LEN-1);
}

void Ntrip_Client::doRequest()
{
	std::string strData = "GET "+ m_strUri +" HTTP / 1.1\r\nUser - Agent: " + m_strAgent  + "\r\nAuthorization: " + m_strAuthorization + "\n\n";
	boost::asio::const_buffers_1 request_buffer(strData.data(), strData.size());
	m_Sock.send(request_buffer);
}

void Ntrip_Client::close()
{
	m_io.post([this]() { m_Sock.close(); });
}

void Ntrip_Client::registerCallBack(std::function<void(char*,int)> func)
{
	callback = std::move(func);
}