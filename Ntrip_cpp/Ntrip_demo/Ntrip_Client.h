#pragma once
#include <string>
#include "define.h"

using namespace boost;
using namespace boost::asio;

class Ntrip_Client
{
public:
	Ntrip_Client(std::string strIp,int nPort,std::string strAgent,std::string strAuthorization,std::string strUri);
	~Ntrip_Client();

	void doRequest();
	void registerCallBack(std::function<void(char*, int)> func);
private:
	boost::asio::io_service& m_io;
	ip::tcp::endpoint m_ep;
	tcp::socket m_Sock;

	std::string m_strIp;
	int m_nPort;
	std::string m_strAgent;
	std::string m_strAuthorization;
	std::string m_strUri;

	char m_buf[MAX_BUFF_LEN];

	std::function<void(char*, int)> callback;
private:
	void onConnect(const boost::system::error_code & ec,sock_ptr pSock);
	void onRead(const boost::system::error_code & ec);
	void close();
};

