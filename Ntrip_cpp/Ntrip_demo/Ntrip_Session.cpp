#include "Ntrip_Session.h"
#include "global.h"

Ntrip_Session::Ntrip_Session(tcp::socket socket)
	:m_socket(std::move(socket))
{
}

Ntrip_Session::~Ntrip_Session()
{
}

void Ntrip_Session::Start()
{
	doRead();
}

void Ntrip_Session::DataStream(char * pdata, int size)
{
	std::memcpy(m_buf, pdata, size);
	doWrite();
}

void Ntrip_Session::doRead()
{
	auto self(shared_from_this());
	m_socket.async_read_some(boost::asio::buffer(m_buf, MAX_BUFF_LEN),
		[this, self](boost::system::error_code ec, std::size_t length)
	{
		if (!ec)
		{
			boost::cmatch mat;
			boost::regex reg("/SF0[1-3]/");
			bool r = boost::regex_match(m_buf, mat, reg);
			if (r)
			{
				std::map<std::string, Ntrip_Client*>::iterator it = ClientMap.find(mat[0]);
				if (it != ClientMap.end())
				{
					Ntrip_Client* pClient = static_cast<Ntrip_Client*>(it->second);
					pClient->registerCallBack(std::bind(&Ntrip_Session::DataStream,this, std::placeholders::_1, std::placeholders::_2));
					pClient->doRequest();
				}
			}
		}
	});
}

void Ntrip_Session::doWrite()
{
	boost::asio::async_write(m_socket,boost::asio::buffer(m_buf, MAX_BUFF_LEN),
		[this](boost::system::error_code ec, std::size_t /*length*/)
	{

	});
}