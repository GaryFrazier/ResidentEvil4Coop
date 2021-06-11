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
    int* playerPointer = GetPlayerPointer();
    Vector3* result = (Vector3*)(*(playerPointer) + 0x94);

    if ((int)result > (int)modBase)
    {
        return result;
    }

    struct Vector3 zero;
    return &zero;
}

float* GetCurrentRotation()
{
    int* playerPointer = GetPlayerPointer();
    float* result = (float*)(*(playerPointer)+0xA4);

    if ((int)result > (int)modBase)
    {
        return result;
    }

    float* zero = 0;
    return zero;
}

Vector3* GetPartnerLocation()
{
    int* partnerPointer = GetPartnerObjectPointer();
    Vector3* result = (Vector3*)(*(partnerPointer)+0x94);

    if ((int)result > (int)modBase)
    {
        return result;
    }

    struct Vector3 zero;
    return &zero;
}

float* GetPartnerRotation()
{
    int* partnerPointer = GetPartnerObjectPointer();
    float* result = (float*)(*(partnerPointer)+0xA4);

    if ((int)result > (int)modBase)
    {
        return result;
    }

    float* zero = 0;
    return zero;
}

short GetCurrentAreaId()
{
    return *(short*)(modBase + 0x85F70C);
}