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

		if (duration < .200)
		{
			Interpolate(&start, durationPtr);
		}
		else
		{
			start = std::clock();

			// setup

			struct Packet packetData;
			Vector3* loc = GetCurrentLocation();

			cout << loc;
			packetData.senderLocation = *loc;

			if (newTestPacket == nullptr)
			{
				previousTestPacket = &packetData;
			}
			else
			{
				previousTestPacket = newTestPacket;
			}

			newTestPacket = &packetData;

			cout << "Partner new location: " << previousTestPacket->senderLocation.x << " " << previousTestPacket->senderLocation.z << "\n";
			MovePartner(&(previousTestPacket->senderLocation));


			duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
		}
		
	}
}

void Interpolate(std::clock_t* start, double* duration)
{
	while (*duration < 0.200)
	{
		cout << "interpolating\n";
		if (newTestPacket != nullptr && previousTestPacket != nullptr)
		{
			cout << "interpolating packet\n";
			struct Vector3 interpolatedLocation;

			interpolatedLocation.x = previousTestPacket->senderLocation.x + (newTestPacket->senderLocation.x - previousTestPacket->senderLocation.x + 1000);
			interpolatedLocation.y = previousTestPacket->senderLocation.y + (newTestPacket->senderLocation.y - previousTestPacket->senderLocation.y);
			interpolatedLocation.z = previousTestPacket->senderLocation.z + (newTestPacket->senderLocation.z - previousTestPacket->senderLocation.z + 1000);

			MovePartner(&(interpolatedLocation));
			cout << "interpolating complete\n";
		}

		Sleep(15);
		*duration = (std::clock() - *start);
	}
}
