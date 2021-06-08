#include "pch.h"
#include "getters.h"

int* GetPlayerPointer()
{
    return (int*)(modBase + 0x857054);
}

int* GetPartnerObjectPointer()
{
    return (int*)(modBase + 0x857060);
}

Vector3* GetCurrentLocation()
{
    int* vectorPointer = GetPlayerPointer();
    Vector3* result = (Vector3*)(*(vectorPointer) + 0x94);

    return result;
}

Vector3* GetPartnerLocation()
{
    int* vectorPointer = GetPartnerObjectPointer();
    Vector3* result = (Vector3*)(*(vectorPointer)+0x94);

    return result;
}