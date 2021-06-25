#include "pch.h"
#include "test.h"

Packet* previousTestPacket = nullptr;
Packet* newTestPacket = nullptr;

void TestLoop()
{
	double duration = 0;
	double* durationPtr = &duration;
	std::clock_t start;

	while (true)
	{

		if (duration < 1)
		{
			//Interpolate(&start, durationPtr);
			Sleep(1000);
			duration = 1;
		}
		else
		{
			start = std::clock();

			// setup

			struct Packet packetData;
			Vector3* loc = GetCurrentLocation();
			float* rot = GetCurrentRotation();
			Enemy* enemyData = GetEnemyData();

			cout << "out of enemy data";
			packetData.senderLocation = *loc;
			packetData.senderEnemyData = enemyData;

			if (rot == 0)
			{
				packetData.senderRotation = 0;
			}
			else
			{
				packetData.senderRotation = *rot;
			}

			Sleep(1000);
			string serialized = Serialize(&packetData);
			cout << "out of serialize\n";

			char* p = new char[8192];
			strcpy(p, const_cast<char*>(serialized.c_str()));
			Packet* newPack = Deserialize(p);
			cout << "out of deserialize\n";
			Sleep(1000);

			if (newTestPacket == nullptr)
			{
				previousTestPacket = &packetData;
			}
			else
			{
				previousTestPacket = newTestPacket;
			}

			newTestPacket = &packetData;

			//MovePartner(&(previousTestPacket->senderLocation));
			//RotatePartner(previousTestPacket->senderRotation);
			SetEnemyData(previousTestPacket->senderEnemyData);

			duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
		}
		
	}
}

void Interpolate(std::clock_t* start, double* duration)
{
	while (*duration < 1)
	{
		if (newTestPacket != nullptr && previousTestPacket != nullptr)
		{
			struct Vector3 interpolatedLocation;

			interpolatedLocation.x = previousTestPacket->senderLocation.x + (newTestPacket->senderLocation.x - previousTestPacket->senderLocation.x + 1000);
			interpolatedLocation.y = previousTestPacket->senderLocation.y + (newTestPacket->senderLocation.y - previousTestPacket->senderLocation.y);
			interpolatedLocation.z = previousTestPacket->senderLocation.z + (newTestPacket->senderLocation.z - previousTestPacket->senderLocation.z + 1000);

			MovePartner(&(interpolatedLocation));

			RotatePartner(previousTestPacket->senderRotation + (newTestPacket->senderRotation - previousTestPacket->senderRotation));
		}

		Sleep(50);
		*duration = (std::clock() - *start);
	}
}
