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
    Vector3* z = &zero;
    return z;
}

float* GetCurrentRotation()
{
    int* playerPointer = GetPlayerPointer();
    float* result = (float*)(*(playerPointer)+0xA4);

    if ((int)result > (int)modBase)
    {
        return result;
    }

    float zero;
    float* z = &zero;
    return z;
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
Enemy* GetEnemyData()
{
    int* baseEnemyPointer = (int*)(modBase + 0x7FDB18);

    struct Enemy baseEnemy;
    if (*baseEnemyPointer == 0x0)
    {
        return 0;
    }
    else
    {
        Vector3* pos = (Vector3*)(*(baseEnemyPointer)+0x94);
        if ((int)pos > (int)modBase)
        {
            baseEnemy.pos = *pos;
        }
        
        float* rot = (float*)(*(baseEnemyPointer)+0xA4);
        if ((int)rot > (int)modBase)
        {
            baseEnemy.rot = *rot;
        }

        short* health = (short*)(*(baseEnemyPointer)+0x324);
        if ((int)health > (int)modBase)
        {
            baseEnemy.health = *health;
        }

        int* nextEnemyPtr = (int*)(*(baseEnemyPointer)+0x8);
        Enemy* currentEnemy = &baseEnemy;

        while (*nextEnemyPtr != 0x0)
        {
            struct Enemy nextEnemy;

            pos = (Vector3*)(*(nextEnemyPtr)+0x94);

            if ((int)pos > (int)modBase)
            {
                nextEnemy.pos = *pos;
            }

            rot = (float*)(*(nextEnemyPtr)+0xA4);
            if ((int)rot > (int)modBase)
            {
                nextEnemy.rot = *rot;
            }

            health = (short*)(*(nextEnemyPtr)+0x324);
            if ((int)health > (int)modBase)
            {
                nextEnemy.health = *health;
            }
            
            nextEnemyPtr = (int*)(*(nextEnemyPtr)+0x8);

            currentEnemy->nextEnemy = new Enemy(nextEnemy);

            currentEnemy = currentEnemy->nextEnemy;
        }

        nextEnemyPtr = (int*)(*(baseEnemyPointer)+0x8);
        Enemy* currentEnemy2 = &baseEnemy;

    }

    return new Enemy(baseEnemy);
}