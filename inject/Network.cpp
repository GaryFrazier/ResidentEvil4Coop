#include "pch.h"
#include <ctime>
#include <iostream>           // For cerr and cout
#include <cstdlib>            // For atoi()
#include "PracticalSocket.h"  // For Socket, ServerSocket, and SocketException
#include "Network.h"

using namespace std;

bool isServer;
bool good = true;
const unsigned int RCVBUFSIZE = 2048;    // Size of receive buffer

// timer for packet sends
std::clock_t start;
double duration = 0;

// initialize structs for transfer
char* outgoingData = new char[sizeof(Packet)];
struct Packet dummyOutgoingPacket;
Packet* currentOutgoingPacket = &dummyOutgoingPacket;

char* incomingData = new char[sizeof(Packet)];
struct Packet dummyIncomingPacket;
Packet* currentIncomingPacket = &dummyIncomingPacket;

char buffer[RCVBUFSIZE + 1];    // Buffer for echo string + \0
int bytesReceived = 0;              // Bytes read on each recv()

void InitializeNetwork()
{
	cout << "Welcome to the Resident Evil 4 Multiplayer Mod, by Gary Frazier\n\nIf Capcom is reading this, please hire me :)\n\nYou can find the code for the mod here: https://github.com/GaryFrazier\n";
	cout << "\nIMPORTANT: Both players must install hamachi and be on the same network for you to connect, get hamachi here: https://www.vpn.net/\n";
	cout << "\nSETUP\n\n";
	cout << "Are you the host? Please type 'y' for yes, 'n' for no...\n";
	char host;
	cin >> host;
	
	// determine if host and start connecting
	if (host == 'y')
	{
		cout << "\nYou are the host!\n";
		cout << "\nWaiting on a client to connect to your hamachi IPv4 address...\n";
		isServer = true;

		try
		{
			TCPServerSocket servSock(5555);     // Server Socket object

			for (;;)
			{   // Run forever
				HandleTCPClient(servSock.accept());       // Wait for a client to connect
			}
		}
		catch (SocketException& e)
		{
			cerr << e.what() << endl;
			exit(1);
		}
		// NOT REACHED

		while (good) 
		{
		};

	}
	else
	{
		cout << "\nYou are the client!\n";
		isServer = false;

		string ip;
		cout << "Please Enter the hamachi IPv4 address of the host...\n";
		cin >> ip;
		cout << "\nAttempting connection, if after 10 seconds it is not successful, the connection has failed...\n";
		cout << ip;

		try
		{
			// Establish connection with the echo server
			TCPSocket sock(ip, 5555);

			while (true)
			{
				if (duration < .200)
				{
					Sleep(200 - (duration * 1000));
				}

				start = std::clock();

				struct Packet currentOutgoingPacketData;
				currentOutgoingPacketData.senderLocation = *GetCurrentLocation();
				currentOutgoingPacket = &currentOutgoingPacketData;

				Serialize(currentOutgoingPacket, outgoingData);
				sock.send(outgoingData, strlen(outgoingData));

				// Receive up to the buffer size bytes from the sender
				if ((bytesReceived = (sock.recv(buffer, RCVBUFSIZE))) <= 0)
				{
					cerr << "Unable to read";
					exit(1);
				}

				Deserialize(incomingData, currentIncomingPacket);
				cout << "packet received:\n";
				cout << "Partner new location: " << currentIncomingPacket->senderLocation.x << " " << currentIncomingPacket->senderLocation.z << "\n";

				duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
			}

		}
		catch (SocketException& e)
		{
			cerr << e.what() << endl;
			exit(1);
		}

	}
}


//server loop
void HandleTCPClient(TCPSocket* sock)
{
	cout << "Handling client ";
	try
	{
		cout << sock->getForeignAddress() << ":";
	}
	catch (SocketException e)
	{
		cerr << "Unable to get foreign address" << endl;
	}
	try
	{
		cout << sock->getForeignPort();
	}
	catch (SocketException e)
	{
		cerr << "Unable to get foreign port" << endl;
	}
	cout << endl;

	// Send received string and receive again until the end of transmission
	char echoBuffer[RCVBUFSIZE];
	int recvMsgSize;
	// Zero means end of transmission
	while (true)
	{
		if (duration < .200)
		{
			Sleep(200 - (duration * 1000));
		}

		start = std::clock();

		struct Packet currentOutgoingPacketData;
		currentOutgoingPacketData.senderLocation = *GetCurrentLocation();
		currentOutgoingPacket = &currentOutgoingPacketData;

		Serialize(currentOutgoingPacket, outgoingData);
		sock->send(outgoingData, strlen(outgoingData));

		// Receive up to the buffer size bytes from the sender
		if ((bytesReceived = (sock->recv(buffer, RCVBUFSIZE))) <= 0)
		{
			cerr << "Unable to read";
			exit(1);
		}

		Deserialize(incomingData, currentIncomingPacket);
		cout << "packet received:\n";
		cout << "Partner new location: " << currentIncomingPacket->senderLocation.x << " " << currentIncomingPacket->senderLocation.z << "\n";

		duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	}
	delete sock;
}

void Serialize(Packet* msgPacket, char *data)
{
	memcpy(msgPacket, &data, sizeof(Packet));
}

void Deserialize(char *data, Packet* msgPacket)
{
	memcpy(&data, msgPacket, sizeof(Packet));
}