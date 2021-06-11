#pragma once
#include "Network.h"

string Serialize(Packet* msgPacket);
Packet* Deserialize(char* data);

void PopulateBase(Packet* packet)
void PopulateClientPacket(Packet* packet);
void PopulateServerPacket(Packet* packet);