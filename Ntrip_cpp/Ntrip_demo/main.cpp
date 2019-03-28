#include "client_main.cpp"
#include "server_main.cpp"

#define AS_CLIENT

int main(int argc, char* argv[])
{
#ifdef AS_CLIENT
	StartClient();
#else
	StartServer();
#endif // AS_CLIENT

	return 0;
}