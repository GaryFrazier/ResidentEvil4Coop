#pragma once

struct Packet
{
	Vector3 senderLocation;
} packet;

extern bool isServer;

void InitializeNetwork();
void HandleData(const std::string& data);
void Serialize(Packet* msgPacket, char* data);
void Deserialize(char* data, Packet* msgPacket);