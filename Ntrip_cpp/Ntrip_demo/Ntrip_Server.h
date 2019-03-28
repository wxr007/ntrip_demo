#pragma once
#include <string>

using namespace boost;
using namespace boost::asio;

class Ntrip_Server
{
typedef ip::tcp::socket socket_type;
typedef std::shared_ptr<socket_type> sock_ptr;

public:
	Ntrip_Server(boost::asio::io_service& io_service, std::string strIp, int nPort);
	~Ntrip_Server();

	void Start();
private:
	tcp::acceptor m_acceptor;
	tcp::socket m_socket;
	ip::tcp::endpoint m_ep;
private:
	void doAccept(const boost::system::error_code & ec, sock_ptr pSock);
};

