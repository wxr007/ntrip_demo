#include "Ntrip_Server.h"
#include "Ntrip_Session.h" 

Ntrip_Server::Ntrip_Server(boost::asio::io_service& io_service,std::string strIp, int nPort)
	:m_acceptor(io_service,ip::address::from_string(m_strIp), nPort),
	,m_socket(io_service)
{
}


Ntrip_Server::~Ntrip_Server()
{
}

void Ntrip_Server::Start()
{
	doAccept();
}

void Ntrip_Server::doAccept()
{
	m_acceptor.async_accept(m_socket,
		[this](boost::system::error_code ec)
	{
		if (!ec)
		{
			std::make_shared<Ntrip_Session>(std::move(m_socket))->Start();
		}

		doAccept();
	});
}
