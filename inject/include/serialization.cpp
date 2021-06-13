#include "pch.h"
#include "serialization.h"

using namespace nlohmann;

// serialization
string Serialize(Packet* msgPacket)
{
	json j;
	j["senderLocation"] = { { "x", msgPacket->senderLocation.x }, { "y", msgPacket->senderLocation.y }, { "z", msgPacket->senderLocation.z } };
	j["senderRotation"] = msgPacket->senderRotation;
	j["senderAreaId"] = msgPacket->senderAreaId;
	j["senderHealth"] = msgPacket->senderHealth;

	cout << "serialize 1\n";

	if (msgPacket->senderEnemyData != 0x0)
	{
		json firstJsonEnemy;

		firstJsonEnemy["loc"]["x"] = msgPacket->senderEnemyData->pos.x;
		firstJsonEnemy["loc"]["y"] = msgPacket->senderEnemyData->pos.y;
		firstJsonEnemy["loc"]["z"] = msgPacket->senderEnemyData->pos.z;
		firstJsonEnemy["rot"] = msgPacket->senderEnemyData->rot;
		firstJsonEnemy["health"] = msgPacket->senderEnemyData->health;

		j["senderEnemyData"].push_back(firstJsonEnemy);

		Enemy* nextEnemy = msgPacket->senderEnemyData->nextEnemy;

		while (nextEnemy != 0x0 && (int)nextEnemy < 0xCCCCCCCC)
		{
			json jsonEnemy;

			jsonEnemy["loc"]["x"] = nextEnemy->pos.x;
			jsonEnemy["loc"]["y"] = nextEnemy->pos.y;
			jsonEnemy["loc"]["z"] = nextEnemy->pos.z;
			jsonEnemy["rot"] = nextEnemy->rot;
			jsonEnemy["health"] = nextEnemy->health;

			nextEnemy = nextEnemy->nextEnemy;

			j["senderEnemyData"].push_back(jsonEnemy);
		}	
	}
	else
	{
		j["senderEnemyData"] = nullptr;
	}

	cout << "serialize end\n";
	return j.dump();
}

Packet* Deserialize(char* data)
{
	cout << "\n\n---------deserialize BEGIN------\n";

	
	json j = json::parse(data);
	struct Packet packet;
	struct Vector3 senderLocation;
	senderLocation.x = j["senderLocation"]["x"];
	senderLocation.y = j["senderLocation"]["y"];
	senderLocation.z = j["senderLocation"]["z"];
	packet.senderLocation = senderLocation;
	packet.senderRotation = j["senderRotation"];
	packet.senderHealth = j["senderHealth"];
	packet.senderAreaId = j["senderAreaId"];

	struct Enemy baseEnemy;
	if (j["senderEnemyData"] == nullptr)
	{
		packet.senderEnemyData = 0;
	}
	else
	{
		Enemy* currentEnemy = nullptr;

		for (const auto& element : j["senderEnemyData"])
		{
			if (currentEnemy == nullptr)
			{
				currentEnemy = &baseEnemy;
			}
			else
			{
				struct Enemy newEnemy;
				Enemy* newEnemyPtr = &newEnemy;
				currentEnemy->nextEnemy = newEnemyPtr;
				currentEnemy = currentEnemy->nextEnemy;
			}
			
			struct Vector3 enemyLoc;
			enemyLoc.x = element["loc"]["x"];
			enemyLoc.y = element["loc"]["y"];
			enemyLoc.z = element["loc"]["z"];
			currentEnemy->pos = enemyLoc;
			currentEnemy->rot = element["rot"];
			currentEnemy->health = element["health"];
		}

		Enemy* enemyPtr = &baseEnemy;
		packet.senderEnemyData = enemyPtr;
	}
	return new Packet(packet);
}

void PopulateServerPacket(Packet* packet)
{
	PopulateBase(packet);
	packet->senderEnemyData = GetEnemyData();
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

	short* health = GetCurrentHealth();
	if (health == 0)
	{
		packet->senderHealth = 0;
	}
	else
	{
		packet->senderHealth = *health;
	}

	packet->senderAreaId = GetCurrentAreaId();
}