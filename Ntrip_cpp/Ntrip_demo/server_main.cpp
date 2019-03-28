#include "Ntrip_Server.h"
#include "global.h"
#include <cstdlib>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>

void InitClient(boost::asio::io_service& io_service)
{
	Ntrip_Client* client1 = new Ntrip_Client(io_service, "104.42.214.164", 1201, "NTRIP JS Client/0.2", "Basic eXlkZ2lzOnl1ZGFuMjAwOA==", "SF01");
	Ntrip_Client* client2 = new Ntrip_Client(io_service, "104.42.214.164", 1201, "NTRIP JS Client/0.2", "Basic eXlkZ2lzOnl1ZGFuMjAwOA==", "SF02");
	Ntrip_Client* client3 = new Ntrip_Client(io_service, "104.42.214.164", 1201, "NTRIP JS Client/0.2", "Basic eXlkZ2lzOnl1ZGFuMjAwOA==", "SF03");
	ClientMap["SF01"] = client1;
	ClientMap["SF02"] = client2;
	ClientMap["SF03"] = client3;
}


void StartServer()
{
	boost::asio::io_service io_service;

	InitClient(io_service);
	Ntrip_Server* pServer = new Ntrip_Server(io_service,"127.0.0.1", 1201);
	pServer->Start();

	io_service.run();
}