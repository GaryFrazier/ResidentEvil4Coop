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

    if ((int)result > (int)modBase)
    {
        return result;
    }

    struct Vector3 zero;
    return &zero;
}

Vector3* GetPartnerLocation()
{
    int* vectorPointer = GetPartnerObjectPointer();
    Vector3* result = (Vector3*)(*(vectorPointer)+0x94);

    if ((int)result > (int)modBase)
    {
        return result;
    }

    struct Vector3 zero;
    return &zero;
}