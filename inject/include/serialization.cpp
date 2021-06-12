#include "pch.h"
#include "serialization.h"

using namespace nlohmann;

// serialization
string Serialize(Packet* msgPacket)
{
	json j;
	j["senderLocation"] = { { "x", msgPacket->senderLocation.x }, { "y", msgPacket->senderLocation.y }, { "z", msgPacket->senderLocation.z } };
	j["senderRotation"] = msgPacket->senderRotation;

	if (msgPacket->senderEnemyData != 0x0)
	{
		//j["senderEnemyData"] = 
	}

	return j.dump();
}

Packet* Deserialize(char* data)
{
	json j = json::parse(data);
	struct Packet packet;
	struct Vector3 senderLocation;
	senderLocation.x = j["senderLocation"]["x"];
	senderLocation.y = j["senderLocation"]["x"];
	senderLocation.z = j["senderLocation"]["x"];

	packet.senderLocation = senderLocation;
	packet.senderRotation = j["senderRotation"];



	Packet* ptr = &packet;
	return ptr;
}

void PopulateServerPacket(Packet* packet)
{
	PopulateBase(packet);
}

void PopulateClientPacket(Packet* packet)
{
	PopulateBase(packet);
}

void PopulateBase(Packet* packet)
{
	packet->senderLocation = *GetCurrentLocation();
	float* rot = GetCurrentRotation();
	if (rot == 0)
	{
		packet->senderRotation = 0;
	}
	else
	{
		packet->senderRotation = *rot;
	}

	packet->senderAreaId = GetCurrentAreaId();
}