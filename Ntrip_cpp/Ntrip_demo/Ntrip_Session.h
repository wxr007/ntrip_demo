#pragma once
#include <string>
#include "define.h"

using namespace boost;
using namespace boost::asio;

class Ntrip_Session : public std::enable_shared_from_this<Ntrip_Session>
{
public:
	Ntrip_Session(tcp::socket socket);
	~Ntrip_Session();
	void Start();
	void DataStream(char* pdata, int size);
private:
	tcp::socket m_socket;
	char m_buf[MAX_BUFF_LEN];
private:
	void doRead();
	void doWrite();
};

