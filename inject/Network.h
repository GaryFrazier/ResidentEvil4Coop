#pragma once
#include "Network.h"
#include "PracticalSocket.h"  // For Socket, ServerSocket, and SocketException

struct Packet
{
	Vector3 senderLocation;
	float senderRotation;
	short senderAreaId; // used to determine whether or not to sync
} packet;

extern bool isServer;

void InitializeNetwork();
void HandleTCPClient(TCPSocket* sock);

// loop
void MainSocketLoop(TCPSocket* sock);
void ProcessServerPacketOnClient();
void ProcessClientPacketOnServer();
void InterpolateServer(std::clock_t* start, double* duration);
void InterpolateClient(std::clock_t* start, double* duration);

// shared
void InterpolatePartner();