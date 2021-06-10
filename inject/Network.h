#pragma once
#include "Network.h"
#include "PracticalSocket.h"  // For Socket, ServerSocket, and SocketException

struct Packet
{
	Vector3 senderLocation;
} packet;

extern bool isServer;

void InitializeNetwork();
void Serialize(Packet* msgPacket, char* data);
void Deserialize(char* data, Packet* msgPacket);
void HandleTCPClient(TCPSocket* sock);