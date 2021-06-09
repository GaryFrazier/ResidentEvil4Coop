#pragma once
#include "Network.h"
#include "SocketClient.h"
#include "SocketServer.h"

struct Packet
{
	Vector3 senderLocation;
} packet;

extern bool isServer;

void InitializeNetwork();
void messageReceived(messageStruct* s);
void errorOccurred(errorStruct* e);
void Serialize(Packet* msgPacket, char* data);
void Deserialize(char* data, Packet* msgPacket);