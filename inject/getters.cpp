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

// Enemies are stored in a linked list, with a pointer to the next enemy at base+0x8
Enemy* GetEnemies()
{
    int* baseEnemyPointer = (int*)(modBase + 0x7FDB18);

    struct Enemy baseEnemy;
    if (*baseEnemyPointer == 0x0)
    {
        return 0;
    }
    else
    {
        baseEnemy.pos = *(Vector3*)(*(baseEnemyPointer)+0x94);
        baseEnemy.rot = *(float*)(*(baseEnemyPointer)+0xA4);
        baseEnemy.health = *(short*)(*(baseEnemyPointer)+0x324);

        int* nextEnemyPtr = (int*)(*(baseEnemyPointer)+0x8);
        Enemy* currentEnemy = &baseEnemy;

        while (nextEnemyPtr != 0x0)
        {
            struct Enemy nextEnemy;
            nextEnemy.pos = *(Vector3*)(*(nextEnemyPtr)+0x94);
            nextEnemy.rot = *(float*)(*(nextEnemyPtr)+0xA4);
            nextEnemy.health = *(short*)(*(nextEnemyPtr)+0x324);
            nextEnemyPtr = (int*)(*(nextEnemyPtr)+0x8);

            currentEnemy->nextEnemy = &nextEnemy;
            currentEnemy = &nextEnemy;
        }
    }

    Enemy* result = &baseEnemy;
    return result;
}