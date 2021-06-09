#include "pch.h"
#include "Network.h"
using namespace masesk;
bool isServer;


void InitializeNetwork()
{
	std::cout << "Welcome to the Resident Evil 4 Multiplayer Mod, by Gary Frazier\nIf Capcom is reading this, please hire me :)\nYou can find the code for the mod here: https://github.com/GaryFrazier\n";
	std::cout << "\nSETUP\n\n";
	std::cout << "Are you the host? Please type 'y' for yes, 'n' for no...\n";
	char host;
	std::cin >> host;
	
	EasySocket socketManager;

	if (host == 'y')
	{
		std::cout << "\nYou are the host!\n";
		isServer = true;
		socketManager.socketListen("re4mp", 8080, &HandleData);
	}
	else
	{
		std::cout << "\nYou are the client!\n";
		isServer = false;

		std::string ip;
		std::cout << "Please Enter the IP address of the host...\n";
		std::cin >> ip;

		socketManager.socketConnect("re4mp", ip, 8080);
		socketManager.socketSend("test", "Hello from client!");
	}
}

void HandleData(const std::string& data)
{
#ifdef _DEBUG
	std::cout << "Client sent: " + data << std::endl;
#endif
}


void Serialize(Packet* msgPacket, char *data)
{
	memcpy(msgPacket, &data, sizeof(Packet));
}

void Deserialize(char *data, Packet* msgPacket)
{
	memcpy(&data, msgPacket, sizeof(Packet));
}