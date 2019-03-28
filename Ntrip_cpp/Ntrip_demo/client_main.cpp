#include "Ntrip_Client.h"
#include <cstdlib>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>

void StartClient()
{
	try
	{
		boost::asio::io_service io_service;
		std::thread t([&io_service]() { io_service.run(); });

		Ntrip_Client* client = new Ntrip_Client(io_service, "127.0.0.1", 1201, "NTRIP JS Client/0.2", "Basic eXlkZ2lzOnl1ZGFuMjAwOA==", "SF01");
		client->doRequest();

		client->close();
		t.join();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
}