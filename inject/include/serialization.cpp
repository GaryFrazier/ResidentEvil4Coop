#include "pch.h"
#include "serialization.h"

using namespace nlohmann;

// serialization
string Serialize(Packet* msgPacket)
{
	cout << "\n  serialize 1 \n";

	json j;
	j["senderLocation"] = { { "x", msgPacket->senderLocation.x }, { "y", msgPacket->senderLocation.y }, { "z", msgPacket->senderLocation.z } };
	j["senderRotation"] = msgPacket->senderRotation;
	j["senderAreaId"] = msgPacket->senderAreaId;
	j["senderHealth"] = msgPacket->senderHealth;

	cout << "\n  serialize 2 \n";
	if (msgPacket->senderEnemyData != 0x0 && (int)msgPacket->senderEnemyData > (int)modBase && (int)msgPacket->senderEnemyData < 0xCCCCCCCC)
	{
		cout << "\n  serialize 3 \n";
		json firstJsonEnemy;
		firstJsonEnemy["loc"]["x"] = msgPacket->senderEnemyData->pos.x;
		firstJsonEnemy["loc"]["y"] = msgPacket->senderEnemyData->pos.y;
		firstJsonEnemy["loc"]["z"] = msgPacket->senderEnemyData->pos.z;
		firstJsonEnemy["rot"] = msgPacket->senderEnemyData->rot;
		firstJsonEnemy["health"] = msgPacket->senderEnemyData->health;

		j["senderEnemyData"].push_back(firstJsonEnemy);

		cout << "\n  serialize 4 \n";

		Enemy* nextEnemy = msgPacket->senderEnemyData->nextEnemy;
		while (nextEnemy != 0x0 && (int)nextEnemy > (int)modBase && (int)nextEnemy != 0xCCCCCCCC && (int)nextEnemy != (int)0x1)
		{
			cout << "\n  serialize 5 \n";
			json jsonEnemy;

			jsonEnemy["loc"]["x"] = nextEnemy->pos.x;
			jsonEnemy["loc"]["y"] = nextEnemy->pos.y;
			jsonEnemy["loc"]["z"] = nextEnemy->pos.z;
			jsonEnemy["rot"] = nextEnemy->rot;
			jsonEnemy["health"] = nextEnemy->health;

			nextEnemy = nextEnemy->nextEnemy;
			cout << nextEnemy << "\n";
			j["senderEnemyData"].push_back(jsonEnemy);
		}
	}
	else
	{
		j["senderEnemyData"] = nullptr;
	}

	cout << "\n  serialize 6 \n";
	return j.dump();
}

Packet* Deserialize(char* data)
{

	json j = json::parse(data);

	struct Packet packet;
	struct Vector3 senderLocation;

	cout << "\n 1 \n";

	if (!j["senderLocation"].is_null()) {
		if (!j["senderLocation"]["x"].is_null()) {
			senderLocation.x = j["senderLocation"]["x"];
		}

		if (!j["senderLocation"]["y"].is_null()) {
			senderLocation.y = j["senderLocation"]["y"];
		}

		if (!j["senderLocation"]["z"].is_null()) {
			senderLocation.z = j["senderLocation"]["z"];
		}
	}

	packet.senderLocation = senderLocation;
	packet.senderRotation = j["senderRotation"];
	packet.senderHealth = j["senderHealth"];
	packet.senderAreaId = j["senderAreaId"];

	struct Enemy baseEnemy;

	cout << "\n 2 \n";
	if (j["senderEnemyData"] == nullptr)
	{
		cout << "\n 3 \n";

		packet.senderEnemyData = 0;
	}
	else
	{
		cout << "\n 4 \n";
		Enemy* currentEnemy = nullptr;

		for (const auto& element : j["senderEnemyData"])
		{
			if (currentEnemy == nullptr)
			{
				currentEnemy = &baseEnemy;
			}
			else
			{
				Enemy* newEnemyPtr = new Enemy();
				currentEnemy->nextEnemy = newEnemyPtr;
				currentEnemy = newEnemyPtr;
			}

			cout << "\n 5 " << currentEnemy << "\n";

			struct Vector3 enemyLoc;

			if (!element["loc"].is_null()) {
				if (!element["loc"]["x"].is_null()) {
					enemyLoc.x = element["loc"]["x"];
				}

				if (!element["loc"]["y"].is_null()) {
					enemyLoc.y = element["loc"]["y"];
				}

				if (!element["loc"]["z"].is_null()) {
					enemyLoc.z = element["loc"]["z"];
				}
			}

			cout << "\n 5 2 \n";

			currentEnemy->pos = enemyLoc;

			cout << "\n 5 3 \n";

			currentEnemy->rot = element["rot"];

			cout << "\n 5 4 \n";

			currentEnemy->health = element["health"];

			cout << "\n 5 5 \n";
		}

		cout << "\n 6 \n";
		Enemy* enemyPtr = new Enemy(baseEnemy);
		packet.senderEnemyData = enemyPtr;
	}

	cout << "\n 7 \n";
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
	packet->senderEnemyData = GetEnemyData();
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