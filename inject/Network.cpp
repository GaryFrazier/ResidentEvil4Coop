#include "pch.h"
#include "PracticalSocket.h"  // For Socket, ServerSocket, and SocketException

using namespace std;

#pragma region init

bool isServer;
bool good = true;
const unsigned int RCVBUFSIZE = 2048;    // Size of receive buffer

char* incomingData = new char[RCVBUFSIZE];
struct Packet dummyOutgoingPacket;
Packet* currentOutgoingPacket = &dummyOutgoingPacket;

struct Packet dummyIncomingPacket;
Packet* currentIncomingPacket = &dummyIncomingPacket;

char buffer[RCVBUFSIZE + 1];
int bytesReceived = 0;   // Bytes read on each recv()

Packet* previousPacket = nullptr;
Packet* newPacket = nullptr;

void InitializeNetwork()
{
	cout << "Welcome to the Resident Evil 4 Multiplayer Mod, by Gary Frazier\n\nIf Capcom is reading this, please hire me :)\n\nYou can find the code for the mod here: https://github.com/GaryFrazier/ResidentEvil4Coop\n";
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
		InitializeInjection();

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

		while (good) 
		{
		};

	}
	else
	{
		cout << "\nYou are the client!\n";
		isServer = false;
		InitializeInjection();

		string ip;
		cout << "Please Enter the hamachi IPv4 address of the host...\n";
		cin >> ip;
		cout << "\nAttempting connection, if after 10 seconds it is not successful, the connection has failed...\n";
		cout << ip;

		try
		{
			// Establish connection with the echo server
			TCPSocket sock(ip, 5555);
			MainSocketLoop(&sock);
		}
		catch (SocketException& e)
		{
			cerr << e.what() << endl;
			exit(1);
		}

	}
}

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

	MainSocketLoop(sock);
}

#pragma endregion

#pragma region "loop"

void MainSocketLoop(TCPSocket* sock)
{
	double duration = -1;
	double* durationPtr = &duration;
	std::clock_t start;

	while (true)
	{
		if (duration < .200 && duration != -1)
		{
			cout << "\ninterpolate loop 1: ";
			cout << duration;
			if (isServer)
			{
				InterpolateServer(&start, durationPtr);
			}
			else
			{
				InterpolateClient(&start, durationPtr);
			}
		}
		else
		{
			start = std::clock();

			// create packet to send
			struct Packet currentOutgoingPacketData;
			currentOutgoingPacket = &currentOutgoingPacketData;

			if (isServer)
			{
				PopulateServerPacket(currentOutgoingPacket);
			}
			else
			{
				PopulateClientPacket(currentOutgoingPacket);
			}

			string outgoingData = Serialize(currentOutgoingPacket);
			const char* c_outgoingData = outgoingData.c_str();

			// send packet
			sock->send(c_outgoingData, strlen(c_outgoingData));

			// Receive up to the buffer size bytes from the sender
			if ((bytesReceived = (sock->recv(buffer, RCVBUFSIZE))) <= 0)
			{
				cerr << "Unable to read";
				exit(1);
			}

			currentIncomingPacket = Deserialize(incomingData);
			cout << "packet received:\n";

			// process new packet
			if (newPacket == nullptr)
			{
				cout << "new packet null, setting packet:\n";
				previousPacket = currentIncomingPacket;
			}
			else
			{
				cout << "setting previous packet:\n";
				previousPacket = newPacket;
			}

			newPacket = currentIncomingPacket;

			if (ShouldSync(previousPacket))
			{
				if (isServer)
				{
					ProcessClientPacketOnServer();
				}
				else
				{
					ProcessServerPacketOnClient();
				}
			}

			// adjust time for next packet send
			duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
		}
	}
}

// client 

void ProcessServerPacketOnClient()
{
	cout << "moving partner\n";
	MovePartner(&(previousPacket->senderLocation));
	cout << "rotating partner\n";
	RotatePartner(previousPacket->senderRotation);
}

void InterpolateClient(std::clock_t* start, double* duration)
{
	while (*duration < 0.200)
	{
		if (newPacket != nullptr && previousPacket != nullptr)
		{
			InterpolatePartner();
		}

		Sleep(15);
		*duration = (std::clock() - *start);
	}
}

// server 

void ProcessClientPacketOnServer()
{
	cout << previousPacket->senderRotation;
	cout << "moving partner\n";
	MovePartner(&(previousPacket->senderLocation));
	cout << "rotating partner\n";
	RotatePartner(previousPacket->senderRotation);
}

void InterpolateServer(std::clock_t* start, double* duration)
{
	while (*duration < 0.200)
	{
		if (newPacket != nullptr && previousPacket != nullptr && ShouldSync(newPacket))
		{
			InterpolatePartner();
		}
		Sleep(15);
		*duration = (std::clock() - *start);
	}
}

// shared
void InterpolatePartner()
{
	struct Vector3 interpolatedLocation;
	interpolatedLocation.x = previousPacket->senderLocation.x + (newPacket->senderLocation.x - previousPacket->senderLocation.x);
	interpolatedLocation.y = previousPacket->senderLocation.y + (newPacket->senderLocation.y - previousPacket->senderLocation.y);
	interpolatedLocation.z = previousPacket->senderLocation.z + (newPacket->senderLocation.z - previousPacket->senderLocation.z);
	MovePartner(&(interpolatedLocation));
	RotatePartner(previousPacket->senderRotation + (newPacket->senderRotation - previousPacket->senderRotation));
}

bool ShouldSync(Packet* packet)
{
	return packet->senderAreaId == GetCurrentAreaId();
}

#pragma endregion
