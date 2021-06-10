#include "pch.h"
#include "Network.h"
#include "PracticalSocket.h"  // For Socket, ServerSocket, and SocketException
#include <iostream>           // For cerr and cout
#include <cstdlib>            // For atoi()

using namespace std;

bool isServer;
bool good = true;
const unsigned int RCVBUFSIZE = 2048;    // Size of receive buffer

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

			char test[] = "Hello";
			// Send the string to the echo server
			sock.send(test, strlen(test));

			char echoBuffer[RCVBUFSIZE + 1];    // Buffer for echo string + \0
			int bytesReceived = 0;              // Bytes read on each recv()
			int totalBytesReceived = 0;         // Total bytes read
			// Receive the same string back from the server
			cout << "Received: ";               // Setup to print the echoed string
			while (totalBytesReceived < strlen(test))
			{
				// Receive up to the buffer size bytes from the sender
				if ((bytesReceived = (sock.recv(echoBuffer, RCVBUFSIZE))) <= 0)
				{
					cerr << "Unable to read";
					exit(1);
				}
				totalBytesReceived += bytesReceived;     // Keep tally of total bytes
				echoBuffer[bytesReceived] = '\0';        // Terminate the string!
				cout << echoBuffer;                      // Print the echo buffer
			}
			cout << endl;

			// Destructor closes the socket

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

	// Send received string and receive again until the end of transmission
	char echoBuffer[RCVBUFSIZE];
	int recvMsgSize;
	while ((recvMsgSize = sock->recv(echoBuffer, RCVBUFSIZE)) > 0)
	{ // Zero means
// end of transmission
// Echo message back to client
		sock->send(echoBuffer, recvMsgSize);
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