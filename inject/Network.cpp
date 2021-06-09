#include "pch.h"
#include "Network.h"
#include "SocketClient.h"
#include "SocketServer.h"

bool isServer;

using namespace std;
bool good = true;

void InitializeNetwork()
{
	cout << "Welcome to the Resident Evil 4 Multiplayer Mod, by Gary Frazier\n\nIf Capcom is reading this, please hire me :)\n\nYou can find the code for the mod here: https://github.com/GaryFrazier\n";
	cout << "\nIMPORTANT: Both players must install hamachi and be on the same network for you to connect, get hamachi here: https://www.vpn.net/\n";
	cout << "\nSETUP\n\n";
	cout << "Are you the host? Please type 'y' for yes, 'n' for no...\n";
	char host;
	cin >> host;
	
	if (host == 'y')
	{
		cout << "\nYou are the host!\n";
		cout << "\nWaiting on a client to connect to your hamachi IPv4 address...\n";
		isServer = true;
		
		SocketServer server(5555);
		SocketClient client(server.accept());
		client.setErrorCallback(errorOccurred);
		client.setMessageCallback(messageReceived);

		while (good) 
		{
		};

		client.close();
		server.close();
	}
	else
	{
		cout << "\nYou are the client!\n";
		isServer = false;

		string ip;
		cout << "Please Enter the hamachi IPv4 address of the host...\n";
		cin >> ip;
		cout << "\nAttempting connection, if after 10 seconds it is not successful, the connection has failed...\n";
		
		SocketClient client("127.0.0.1", 5555);
		client.setErrorCallback(errorOccurred);
		client.setMessageCallback(messageReceived);
		client.connect();
		client.send("Hello World!");

		while (good) 
		{

		};

		client.close();
	}
}

void messageReceived(messageStruct* s)
{
#ifdef _DEBUG
	cout << "Client sent: " + s->message << endl;
#endif

	cout << "client: " << s->message << endl;
}

void errorOccurred(errorStruct* e)
{
	cout << e->code << " : " << e->message << endl;
	good = false;
}


void Serialize(Packet* msgPacket, char *data)
{
	memcpy(msgPacket, &data, sizeof(Packet));
}

void Deserialize(char *data, Packet* msgPacket)
{
	memcpy(&data, msgPacket, sizeof(Packet));
}