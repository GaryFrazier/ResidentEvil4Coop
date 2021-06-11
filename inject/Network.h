#pragma once
#include "Network.h"
#include "PracticalSocket.h"  // For Socket, ServerSocket, and SocketException

struct Packet
{
	Vector3 senderLocation;
	float senderRotation;
} packet;

extern bool isServer;

void InitializeNetwork();
string Serialize(Packet* msgPacket);
Packet* Deserialize(char* data);
void HandleTCPClient(TCPSocket* sock);

// loop
void MainSocketLoop(TCPSocket* sock);
void PopulateClientPacket(Packet* packet);
void ProcessServerPacketOnClient();
void PopulateServerPacket(Packet* packet);
void ProcessClientPacketOnServer();
void InterpolateServer(std::clock_t* start, double* duration);
void InterpolateClient(std::clock_t* start, double* duration);

// shared
void InterpolatePartner();