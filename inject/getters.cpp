#include "pch.h"
#include "getters.h"

Vector3* GetCurrentLocation()
{
    int* vectorPointer = (int*)(modBase + 0x857054);
    Vector3* result = (Vector3*)(*(vectorPointer) + 0x94);

    return result;
}
